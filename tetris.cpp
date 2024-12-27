#include <iostream>
#include <cassert>

#include "tetris.h"
#include "tetris-constants.h"
#include "piece.h"

Tetris::Tetris()
{
    m_board.resize(BOARD_HEIGHT);
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        m_board[i].resize(BOARD_WIDTH);
    }

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            m_board[i][j] = BOARD_COLOR;
        }
    }

    m_currentPiece = Piece();
    m_nextPieceType = static_cast<PieceType>(GetRandomValue(0, (int) pieceColors.size() - 1));

    m_linesCleared = 0;
    m_score = 0;
    m_gameOver = false;
}

Tetris::Tetris(double gameWidth, double gameHeight) : Tetris()
{
    m_gameWidth = gameWidth;
    m_gameHeight = gameHeight;
}

const std::vector<std::vector<Color>>& Tetris::GetBoard() const
{
    return m_board;
}

bool Tetris::isValidPiece(const std::array<std::string, 4>& block, int x, int y) const
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            int posX = x + c;
            int posY = y + r;
            if (block[r][c] == 'X')
            {
                // check if the block is out of bounds
                if (posX < 0 || posX >= BOARD_WIDTH || posY < 0 || posY >= BOARD_HEIGHT) return false;
                // check if the block is colliding with another block
                if (IsPieceBlock(m_board[posY][posX])) return false;
            }
        }
    }
    return true;
}

bool Tetris::TryMovePiece(int x, int y)
{
    auto& pieceShape = m_currentPiece.blocks;
    int newX = m_currentPiece.x + x, newY = m_currentPiece.y + y;

    if (!isValidPiece(pieceShape, newX, newY))
        return false;

    m_currentPiece.SetPosition(newX, newY);
    return true;
}

bool Tetris::TryRotatePiece(bool clockwise)
{
    auto& pieceShape = m_currentPiece.blocks;
    auto newPieceShape = std::array<std::string, 4>{"0000", "0000", "0000", "0000"};

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            newPieceShape[r][c] = clockwise ? pieceShape[c][3 - r] : pieceShape[3 - c][r];       
        }
    }

    if (!isValidPiece(newPieceShape, m_currentPiece.x, m_currentPiece.y))
        return false;

    pieceShape = newPieceShape;
    return true;
}

void Tetris::MovePieceDown()
{
    static double lastGravityMoveTime = GRAVITY_SPEED_IN_SECONDS;
    lastGravityMoveTime -= GetFrameTime();
    if (lastGravityMoveTime < 0)
    {
        if (!TryMovePiece(0, 1))
        {
            m_currentPiece.hasLanded = true;
        }
        lastGravityMoveTime = GRAVITY_SPEED_IN_SECONDS;
    }
}

inline void Tetris::SpawnNewPiece()
{
    m_currentPiece = Piece(m_nextPieceType);
    m_currentPiece.SetPosition(GetRandomValue(0, BOARD_WIDTH - 4), 0);

    m_nextPieceType = static_cast<PieceType>(GetRandomValue(0, (int) pieceColors.size() - 1));
}

void Tetris::AddPieceToBoard()
{
    auto& pieceShape = m_currentPiece.blocks;
    auto& pieceColor = pieceColors.at(m_currentPiece.pieceType);

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (pieceShape[r][c] == '0') continue;

            int x = m_currentPiece.x + c;
            int y = m_currentPiece.y + r;
            m_board[y][x] = pieceColor;
        }
    }
}

void Tetris::ClearFullRows()
{
    int rowIndex = BOARD_HEIGHT - 1;
    int consecutiveRowsCleared = 0;

    for (int r = BOARD_HEIGHT - 1; r >= 0; r--)
    {
        bool fullRow = true;
        for (int c = 0; c < BOARD_WIDTH; c++)
        {
            if (!IsPieceBlock(m_board[r][c]))
            {
                fullRow = false;
                break;
            }
        }

        if (!fullRow)
        {
            m_board[rowIndex] = m_board[r];
            rowIndex--;

            m_linesCleared += consecutiveRowsCleared;
            m_score += SCORE[consecutiveRowsCleared];
            consecutiveRowsCleared = 0;
        }
        else
        {
            consecutiveRowsCleared++;
        }
    }
    m_linesCleared += consecutiveRowsCleared;
    m_score += SCORE[consecutiveRowsCleared];

    for (int r = 0; r < rowIndex; r++)
    {
        for (int c = 0; c < BOARD_WIDTH; c++)
        {
            m_board[r][c] = BOARD_COLOR;
        }
    }
}

inline bool Tetris::IsPieceBlock(const Color& color) const
{
    return color.a == pieceAlpha;
}

void Tetris::DrawBlock(int x, int y, const Color color) const
{
    constexpr float offset = 0.15;
    constexpr int OFFSET_SIZE = BLOCK_SIZE * offset;

    DrawRectangle(
        x * BLOCK_SIZE + OFFSET_SIZE,
        y * BLOCK_SIZE + OFFSET_SIZE,
        BLOCK_SIZE - 2 * OFFSET_SIZE,
        BLOCK_SIZE - 2 * OFFSET_SIZE,
        WHITE
    );
    DrawRectangleLines(
        x * BLOCK_SIZE,
        y * BLOCK_SIZE,
        BLOCK_SIZE,
        BLOCK_SIZE,
        BLACK
    );
    DrawRectangle(
        x * BLOCK_SIZE,
        y * BLOCK_SIZE,
        BLOCK_SIZE,
        BLOCK_SIZE,
        color
    );

    if (IsPieceBlock(color))
    {
        DrawRectangle(
            x * BLOCK_SIZE + OFFSET_SIZE,
            y * BLOCK_SIZE + OFFSET_SIZE,
            OFFSET_SIZE,
            OFFSET_SIZE,
            WHITE
        );
    }
}

void Tetris::DrawBoard() const
{
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            DrawBlock(j, i, m_board[i][j]);
        }
    }
}

void Tetris::DrawFallingPiece() const
{
    auto& pieceShape = m_currentPiece.blocks;
    auto& pieceColor = pieceColors.at(m_currentPiece.pieceType);

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (pieceShape[r][c] == '0') continue;

            int x = m_currentPiece.x + c;
            int y = m_currentPiece.y + r;

            DrawBlock(x, y, pieceColor);
        }
    }
}

void Tetris::DrawPreviewOfNextPiece() const
{
    auto& nextPieceShape = pieceShapes.at(m_nextPieceType);
    auto& nextPieceColor = pieceColors.at(m_nextPieceType);
    
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (nextPieceShape[r][c] == 'X')
                DrawBlock(c + BOARD_WIDTH + 1, r + 1, nextPieceColor);
            else 
                DrawBlock(c + BOARD_WIDTH + 1, r + 1, PREVIEW_COLOR);
        }
    }
}

void Tetris::PrintStatsToScreen() const
{
    auto lines = "Lines Cleared: " + std::to_string(m_linesCleared);
    DrawText(lines.c_str(), (BOARD_WIDTH + 1) * BLOCK_SIZE, 5 * BLOCK_SIZE + 10, 20, WHITE);
    auto score = "Score: " + std::to_string(m_score);
    DrawText(score.c_str(), (BOARD_WIDTH + 1) * BLOCK_SIZE, 6 * BLOCK_SIZE + 10, 20, WHITE);
}

void Tetris::PrintSettingsToScreen() const
{
    auto settings = "Controls: \n\n"
                    "Move Left: A or Left Arrow\n"
                    "Move Right: D or Right Arrow\n"
                    "Move Down: S or Down Arrow\n"
                    "Rotate Clockwise: W or Up Arrow\n"
                    "Rotate Counter-Clockwise: X\n"
                    "Hard Drop: Space\n"
                    "Restart Game: R\n";

    DrawText(settings, (BOARD_WIDTH + 1) * BLOCK_SIZE, 10 * BLOCK_SIZE + 10, 15, WHITE);
}

void Tetris::Init()
{
    assert(pieceShapes.size() == pieceColors.size());
}

void Tetris::Input()
{
    static double lastXInputTime = MOVE_LEFT_RIGHT_SPEED_IN_SECONDS;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        lastXInputTime -= GetFrameTime();
        if (lastXInputTime < 0)
        {
            TryMovePiece(1, 0);
            lastXInputTime = MOVE_LEFT_RIGHT_SPEED_IN_SECONDS;
        }
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        lastXInputTime -= GetFrameTime();
        if (lastXInputTime < 0)
        {
            TryMovePiece(-1, 0);
            lastXInputTime = MOVE_LEFT_RIGHT_SPEED_IN_SECONDS;
        }
    }

    static double lastYInputTime = MOVE_DOWN_SPEED_IN_SECONDS;
    if (IsKeyPressed(KEY_SPACE))
    {
        while (TryMovePiece(0, 1)) {}
        m_currentPiece.hasLanded = true;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        lastYInputTime -= GetFrameTime();
        if (lastYInputTime < 0)
        {
            if (!TryMovePiece(0, 1)) m_currentPiece.hasLanded = true;
            lastYInputTime = MOVE_DOWN_SPEED_IN_SECONDS;
        }
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        TryRotatePiece(false);
    }
    else if (IsKeyPressed(KEY_X))
    {
        TryRotatePiece(true);
    }
    
    // for debugging purposes only (change block type)
    if (IsKeyPressed(KEY_T)) 
    {
        int pieceType = (m_currentPiece.pieceType + 1) % pieceColors.size();
        m_currentPiece.pieceType = static_cast<PieceType>(pieceType);
        m_currentPiece.blocks = pieceShapes.at(m_currentPiece.pieceType);
    }

    // reset the game
    if (IsKeyPressed(KEY_R))
    {
        *this = Tetris(m_gameWidth, m_gameHeight);
    }
    
}

void Tetris::Update()
{
    if (m_gameOver) return;
    
    MovePieceDown();

    if (m_currentPiece.hasLanded)
    {
        if (m_currentPiece.y == 0)
        {
            m_gameOver = true;
        }
        else
        {
            AddPieceToBoard();
            ClearFullRows();
            SpawnNewPiece();
        }
    }
}

void Tetris::Render()
{
    ClearBackground(GRAY);
    DrawBoard();
    DrawFallingPiece();
    DrawPreviewOfNextPiece();

    PrintStatsToScreen();
    PrintSettingsToScreen();
}
