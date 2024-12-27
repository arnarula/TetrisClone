#pragma once

#include <vector> 
#include "raylib.h"
#include "game.h"
#include "piece.h"

class Tetris : public Game
{
    private:
        std::vector<std::vector<Color>> m_board;
        Piece m_currentPiece;
        PieceType m_nextPieceType;

        int m_linesCleared;
        int m_score;
        bool m_gameOver;

    public:
        Tetris();
        Tetris(double gameWidth, double gameHeight);

        const std::vector<std::vector<Color>>& GetBoard() const;  

        // input helper functions
        bool isValidPiece(const std::array<std::string, 4>& block, int x, int y) const;
        bool TryMovePiece(int x, int y);
        bool TryRotatePiece(bool clockwise);

        // update and game logic functions
        void MovePieceDown();
        void ClearFullRows();
        void AddPieceToBoard();
        void SpawnNewPiece();

        // render helper functions
        bool IsPieceBlock(const Color& color) const;
        void DrawBlock(int x, int y, const Color color) const;
        void DrawFallingPiece() const;
        void DrawPreviewOfNextPiece() const;
        void DrawBoard() const;
        
        void PrintStatsToScreen() const;
        void PrintSettingsToScreen() const;

        void Init() override;
        void Input() override;
        void Update() override;
        void Render() override;
};
