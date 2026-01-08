#pragma once

#include "enemy.h"
#include <vector>
#include <SDL.h>
class Player;

class Slime : public Enemy
{
public:
    Slime();
    virtual ~Slime();

    bool Initialise(Renderer& renderer, int tileX, int tileY, bool vertical);

    void FindPathToPlayer(Player* player);
    void Process(float deltaTime, Player* player);  // New override method

private:
    std::vector<SDL_Point> mPath;
    float mMoveTimer = 0.0f;
    float mMoveInterval = 1.0f;  // Move every 0.5 seconds
    bool CanMoveTo(int tileX, int tileY);
};
