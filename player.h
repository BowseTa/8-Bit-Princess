//player.h
#pragma once

#include "sprite.h"
#include "renderer.h"
#include "animatedsprite.h"
#include <SDL.h>

#include <vector>//

#include <fmod.hpp>

class LevelLoader; // Forward declaration

class Player {
public:
    Player();
    ~Player();

    bool Initialise(Renderer& renderer);
    void Process(float deltaTime, Renderer& renderer, const Uint8* keyState, const std::vector<class Enemy*>& enemies);//
    void Draw(Renderer& renderer);

    void SetLevel(LevelLoader* level);
    void SetPosition(float x, float y);


    float GetX() const;
    float GetY() const;
    SDL_Rect GetBoundingBox(float x, float y) const;
    SDL_Rect GetAttackBox() const;

    int GetHP() const;                 // ✅ ADDED
    void TakeDamage(int amount);      // ✅ ADDED

    // ✅ ADD THIS:
    bool IsAlive() const { return mIsAlive; }

    bool HasKey() const { return mHasKey; }//
    int GetHealth() const { return mHP; }//
    int GetCoinCount() const { return mCoinCount; }
    void AddCoin(int amount) { mCoinCount += amount; }
    void AddHP(int amount) { mHP += amount; }


private:
    Sprite* mSprite;
    AnimatedSprite* mExplosionSprite;

    Texture* mStandTexture;
    Texture* mAttackTexture;
    Texture* mStandTextureUp;
    Texture* mStandTextureDown;
    Texture* mAttackTextureUp;
    Texture* mAttackTextureDown;

    float mX, mY;
    float mSpeed;
    int mTileX, mTileY;
    bool mCanMove;

    bool mIsAttacking;
    float mAttackTimer;
    float mAttackDuration;
    bool mPrevAttackPressed;

    float mExplosionTimer;
    float mExplosionDuration;

    LevelLoader* mLevel;
    bool mFacingLeft;
    int mLastMoveDirX, mLastMoveDirY;

    int mHP;

    float mInvincibleTimer = 0.0f;
    float mInvincibleDuration = 0.5f;
    bool mIsAlive = true;//
    
    //
    bool mIsRotating = false;
    float mRotateTimer = 0.0f;
    float mRotateDuration = 2.0f;  // 2 seconds spin
    float mRotateSpeed = 360.0f;   // Degrees per second
    //
    bool mHasKey = false;

    FMOD::Sound* mCoinSound = nullptr;
    FMOD::Sound* mItemSound = nullptr;

    FMOD::Sound* mAttackSound;
    FMOD::Sound* mDoorSound;

    bool CheckCollision(float x, float y);
    int mCoinCount = 0;
};
