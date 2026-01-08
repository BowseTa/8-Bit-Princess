//enemy.h
#pragma once

#include "sprite.h"
#include <SDL.h>

class Renderer;
class Texture;
class LevelLoader;

class Enemy {
public:
    Enemy();
    virtual ~Enemy();  // ✅ Enables dynamic_cast

    virtual bool Initialise(Renderer& renderer, int tileX, int tileY, bool vertical = false);
    virtual void Process(float deltaTime);
    virtual void Draw(Renderer& renderer);

    void SetLevel(LevelLoader* level);
    void SetPosition(float x, float y);

    void TakeDamage(int amount);
    bool IsAlive() const;
    SDL_Rect GetBoundingBox() const;  // ✅ CORRECTLY DECLARED
    int GetAttackPower() const { return mAttackPower; }

protected:
    Sprite* mSprite;
    float mX, mY;
    int mTileX, mTileY;
    float mSpeed;
    int mDirection;
    bool mVerticalMove;
    int mVerticalDir;

    float mStepTimer;
    float mStepInterval;

    LevelLoader* mLevel;

    int mHP;
    bool mIsAlive;

    int mAttackPower = 1;
};
