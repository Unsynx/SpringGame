#pragma once

#include "SceneManager.h"


class GameDataScene : public Scene {
    private:
        int highScore = 0;
        int lastScore;
    public:
        GameDataScene() : Scene("Data", DATA_CONTAINER) {}
        void setLastScore(int score) { 
            lastScore = score;
            if (lastScore > highScore) highScore = lastScore;
        }
        int getLastScore() { return lastScore; }
        int getHighScore() { return highScore; }
};