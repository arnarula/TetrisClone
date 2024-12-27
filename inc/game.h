#pragma once

class Game {
    public:
        Game();
        Game(double gameWidth, double gameHeight);
        
        virtual void Init();
        void Run();

        void GameLoop();
        virtual void Input();
        virtual void Update();
        virtual void Render();

    protected:
        double m_gameWidth;
        double m_gameHeight;
};
