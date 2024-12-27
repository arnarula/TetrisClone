#include "game.h"
#include "raylib.h"
#include <iostream>

Game::Game()
{
    m_gameWidth = 600;
    m_gameHeight = 800;
}

Game::Game(double gameWidth, double gameHeight)
{
    m_gameWidth = gameWidth;
    m_gameHeight = gameHeight;
}

void Game::Run()
{
    InitWindow(m_gameWidth, m_gameHeight, "");
    SetTargetFPS(60);
    Init();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        GameLoop();
        EndDrawing();
    }

    CloseWindow();
}

void Game::GameLoop()
{
    Input();
    Update();
    Render();
}

void Game::Init()
{
    
}

void Game::Input()
{
    
}

void Game::Update()
{
    
}

void Game::Render()
{
  
}
