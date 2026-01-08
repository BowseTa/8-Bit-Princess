// player.cpp
#include "player.h"
#include "renderer.h"
#include "sprite.h"
#include "texture.h"
#include "texturemanager.h"
#include "levelloader.h"
#include "tile.h"
#include "enemy.h"
#include <SDL.h>
#include <algorithm>
#include <iostream>

#include "game.h"

Player::Player()
    : mSprite(nullptr), mX(100.0f), mY(100.0f), mSpeed(200.0f), mLevel(nullptr),
    mFacingLeft(false), mLastMoveDirX(0), mLastMoveDirY(0), mHP(3)
{
}

Player::~Player()
{
    delete mSprite;

    if (mCoinSound) mCoinSound->release();
    if (mItemSound) mItemSound->release();

    if (mAttackSound) mAttackSound->release();
    if (mDoorSound) mDoorSound->release();
}

bool Player::Initialise(Renderer& renderer)
{
    mStandTexture = renderer.GetTextureManager()->GetTexture("assets/Sprites/charactor_stand.png");
    mAttackTexture = renderer.GetTextureManager()->GetTexture("assets/Sprites/charactor_attack.png");

    mStandTextureUp = renderer.GetTextureManager()->GetTexture("assets/Sprites/charactor_stand_up.png");
    mStandTextureDown = renderer.GetTextureManager()->GetTexture("assets/Sprites/charactor_stand_down.png");
    mAttackTextureUp = renderer.GetTextureManager()->GetTexture("assets/Sprites/charactor_attack_up.png");
    mAttackTextureDown = renderer.GetTextureManager()->GetTexture("assets/Sprites/charactor_attack_down.png");

    FMOD::System* fmod = renderer.GetFMODSystem();

    fmod->createSound("assets/Audio/coin2.wav", FMOD_DEFAULT, 0, &mCoinSound);
    fmod->createSound("assets/Audio/item.wav", FMOD_DEFAULT, 0, &mItemSound);

    fmod->createSound("assets/Audio/attack.wav", FMOD_DEFAULT, 0, &mAttackSound);
    fmod->createSound("assets/Audio/door.wav", FMOD_DEFAULT, 0, &mDoorSound);

    if (!mStandTexture || !mAttackTexture ||
        !mStandTextureUp || !mStandTextureDown ||
        !mAttackTextureUp || !mAttackTextureDown)
        return false;

    mSprite = new Sprite();
    float scale = 64.0f / mStandTexture->GetWidth();
    mSprite->SetScale(scale);
    if (!mSprite->Initialise(*mStandTexture)) return false;

    mTileX = 1;
    mTileY = 1;
    mCanMove = true;

    mX = mTileX * 64.0f;
    mY = mTileY * 64.0f;
    mSprite->SetX(static_cast<int>(mX));
    mSprite->SetY(static_cast<int>(mY));

    mIsAttacking = false;
    mAttackDuration = 0.2f;
    mAttackTimer = 0.0f;

    Texture* explosionTex = renderer.GetTextureManager()->GetTexture("assets/Sprites/explosion.png");
    if (!explosionTex) return false;

    mExplosionSprite = new AnimatedSprite();
    mExplosionSprite->SetScale(1.0f);
    mExplosionSprite->Initialise(*explosionTex);
    mExplosionSprite->SetupFrames(64, 64);
    mExplosionSprite->SetFrameDuration(0.1f);
    mExplosionSprite->SetLooping(false);
    mExplosionDuration = mExplosionSprite->GetFrameCount() * 0.1f;

    mPrevAttackPressed = false;

    return true;
}

void Player::SetLevel(LevelLoader* level)
{
    mLevel = level;
}

SDL_Rect Player::GetBoundingBox(float x, float y) const
{
    SDL_Rect rect;
    rect.w = 40;
    rect.h = 40;
    rect.x = static_cast<int>(x);
    rect.y = static_cast<int>(y);
    return rect;
}

SDL_Rect Player::GetAttackBox() const
{
    SDL_Rect attackBox;
    attackBox.w = 64;
    attackBox.h = 64;

    if (mLastMoveDirY == -1) // Up
    {
        attackBox.x = static_cast<int>(mX);
        attackBox.y = static_cast<int>(mY - 64);
    }
    else if (mLastMoveDirY == 1) // Down
    {
        attackBox.x = static_cast<int>(mX);
        attackBox.y = static_cast<int>(mY + 64);
    }
    else // Left/Right
    {
        attackBox.x = static_cast<int>(mFacingLeft ? mX - 64 : mX + 64);
        attackBox.y = static_cast<int>(mY);
    }

    return attackBox;
}

bool Player::CheckCollision(float x, float y)
{
    SDL_Rect playerBox = GetBoundingBox(x, y);
    for (Tile* tile : mLevel->GetTiles())
    {
        SDL_Rect tileBox = tile->GetBoundingBox();
        if (tile->IsSolid() && SDL_HasIntersection(&playerBox, &tileBox))
            return true;
    }
    return false;
}

void Player::Process(float deltaTime, Renderer& renderer, const Uint8* keyState, const std::vector<Enemy*>& enemies)
{
    // --- Spinning Animation (after door is opened) ---
    if (mIsRotating)
    { 
        mRotateTimer += deltaTime;

        float angle = mSprite->GetAngle();
        angle += mRotateSpeed * deltaTime;
        mSprite->SetAngle(angle);

        if (mRotateTimer >= mRotateDuration)
        {
            Game::GetInstance().SetWin();  // Go to win screen
            Game::GetInstance().mStageTime = SDL_GetTicks() / 1000.0f - Game::GetInstance().mStageStartTime;
            return;
        }

        return;  // Skip input, movement, etc. during spin
    }
    //

    bool isAttackPressed = keyState[SDL_SCANCODE_A];

    if (isAttackPressed && !mPrevAttackPressed)
    {
        mIsAttacking = true;

        if (mAttackSound)
        {
            renderer.GetFMODSystem()->playSound(mAttackSound, 0, false, 0);
        }

        mAttackTimer = mAttackDuration;
        if (mLastMoveDirY == -1)
            mSprite->SetTexture(*mAttackTextureUp);
        else if (mLastMoveDirY == 1)
            mSprite->SetTexture(*mAttackTextureDown);
        else
            mSprite->SetTexture(*mAttackTexture);

        mExplosionTimer = mExplosionDuration;
        mExplosionSprite->Restart();
        mExplosionSprite->Animate();
        mExplosionSprite->SetScaleX(mFacingLeft ? -1.0f : 1.0f);

        SDL_Rect attackBox = GetAttackBox();
        for (Enemy* enemy : enemies)
        {
            if (enemy->IsAlive())
            {
                SDL_Rect enemyBox = enemy->GetBoundingBox();
                if (SDL_HasIntersection(&attackBox, &enemyBox))
                {
                    enemy->TakeDamage(1);
                }
            }
        }
    }

    mPrevAttackPressed = isAttackPressed;

    //
    if (mIsAlive && mInvincibleTimer <= 0.0f)
    {
        SDL_Rect playerBox = GetBoundingBox(mX, mY);
        for (Enemy* enemy : enemies)
        {
            if (enemy->IsAlive())
            {
                SDL_Rect enemyBox = enemy->GetBoundingBox();
                if (SDL_HasIntersection(&playerBox, &enemyBox))
                {
                    TakeDamage(enemy->GetAttackPower());
                    break;
                }
            }
        }
    }

    for (Tile* tile : mLevel->GetTiles())
    {
        if (tile->IsCollectible() && !tile->IsCollected())
        {
            SDL_Rect tileBox = tile->GetBoundingBox();
            SDL_Rect playerBox = GetBoundingBox(mX, mY);
            if (SDL_HasIntersection(&playerBox, &tileBox))
            {
                tile->SetCollected(true);

                switch (tile->GetType())
                {
                case TileType::Coin:
                    AddCoin(1);  // ✅ count up
                    Game::GetInstance().mScore += 100;
                    if (mCoinSound) renderer.GetFMODSystem()->playSound(mCoinSound, 0, false, 0);
                    break;
                case TileType::Key:
                    mHasKey = true;  // ✅ Now the player has the key
                    std::cout << "Key collected!" << std::endl;
                    if (mItemSound) renderer.GetFMODSystem()->playSound(mItemSound, 0, false, 0);
                    break;
                case TileType::Heart:
                    AddHP(1);  // ✅ add 1 HP
                    std::cout << "Heart collected! HP: " << mHP << std::endl;
                    if (mItemSound) renderer.GetFMODSystem()->playSound(mItemSound, 0, false, 0);
                    break;
                default:
                    break;
                }

                std::cout << "Collected item at (" << tile->GetX() << ", " << tile->GetY() << ")" << std::endl;
            }
        }
    }

    //
    if (!mCanMove)
    {
        if (!(keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT] ||
            keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_DOWN]))
        {
            mCanMove = true;
        }
        return;
    }

    //
    if (mInvincibleTimer > 0.0f)
        mInvincibleTimer -= deltaTime;

    if (!mIsAlive)
        return;  // Skip all further processing if dead


    int dx = 0, dy = 0;
    if (keyState[SDL_SCANCODE_LEFT]) { dx = -1; }
    else if (keyState[SDL_SCANCODE_RIGHT]) { dx = 1; }
    else if (keyState[SDL_SCANCODE_UP]) { dy = -1; }
    else if (keyState[SDL_SCANCODE_DOWN]) { dy = 1; }

    if (dx != 0 || dy != 0)
    {
        // Track last movement direction
        mLastMoveDirX = dx;
        mLastMoveDirY = dy;

        // Change standing texture based on direction
        if (dy == -1)
            mSprite->SetTexture(*mStandTextureUp);
        else if (dy == 1)
            mSprite->SetTexture(*mStandTextureDown);
        else if (dx != 0)
            mSprite->SetTexture(*mStandTexture);

        // Flip the sprite
        if (dx != 0)
        {
            bool newFacingLeft = dx < 0;
            if (newFacingLeft != mFacingLeft)
            {
                mFacingLeft = newFacingLeft;
                float currentScale = mSprite->GetScale();
                mSprite->SetScaleX(mFacingLeft ? -std::abs(currentScale) : std::abs(currentScale));
                mCanMove = false;  // prevent moving in same frame
                return;
            }
        }

        int targetTileX = mTileX + dx;
        int targetTileY = mTileY + dy;
        float testX = targetTileX * 64;
        float testY = targetTileY * 64;

        if (!CheckCollision(testX, testY))
        {
            mTileX = targetTileX;
            mTileY = targetTileY;
            mCanMove = false;
        }
    }

    mX = mTileX * 64.0f;
    mY = mTileY * 64.0f;

    if (mSprite)
    {
        mSprite->SetX(static_cast<int>(mX));
        mSprite->SetY(static_cast<int>(mY));
    }

    // ✅ Check for win condition: player at Door and presses Enter
    for (Tile* tile : mLevel->GetTiles())
    {
        if (tile->GetType() == TileType::Door)
        {
            SDL_Rect playerBox = GetBoundingBox(mX, mY);
            SDL_Rect doorBox = tile->GetBoundingBox();

            //
            if (SDL_HasIntersection(&playerBox, &doorBox) && keyState[SDL_SCANCODE_RETURN] && mHasKey)
            {
                if (!mIsRotating)
                {
                    if (mDoorSound)
                    {
                        renderer.GetFMODSystem()->playSound(mDoorSound, 0, false, 0);
                    }

                    mIsRotating = true;
                    mRotateTimer = 0.0f;
                    mSprite->SetAngle(0.0f);  // Start fresh rotation
                }

                return;  // Don't process anything else this frame
            }
            //

        }
    }

    if (mExplosionTimer > 0.0f)
    {
        mExplosionTimer -= deltaTime;
        mExplosionSprite->Process(deltaTime);
    }

    if (mIsAttacking)
    {
        mAttackTimer -= deltaTime;
        if (mAttackTimer <= 0.0f)
        {
            mIsAttacking = false;
            if (mLastMoveDirY == -1)
                mSprite->SetTexture(*mStandTextureUp);
            else if (mLastMoveDirY == 1)
                mSprite->SetTexture(*mStandTextureDown);
            else
                mSprite->SetTexture(*mStandTexture);
        }
    }
}

void Player::Draw(Renderer& renderer)
{
    if (!mIsAlive)
        return;

    if (mSprite) mSprite->Draw(renderer);

    if (mExplosionTimer > 0.0f && mExplosionSprite)
    {
        int explosionX = static_cast<int>(mX);
        int explosionY = static_cast<int>(mY);

        if (mLastMoveDirY == -1) // Up
        {
            explosionY -= 64;
        }
        else if (mLastMoveDirY == 1) // Down
        {
            explosionY += 64;
        }
        else // Left/Right
        {
            explosionX = static_cast<int>(mFacingLeft ? mX - 64 : mX + 64);
        }

        mExplosionSprite->SetX(explosionX);
        mExplosionSprite->SetY(explosionY);
        mExplosionSprite->Draw(renderer);
    }
}

float Player::GetX() const { return mX; }

float Player::GetY() const { return mY; }

int Player::GetHP() const { return mHP; }

void Player::TakeDamage(int dmg)
{
    if (!mIsAlive || mInvincibleTimer > 0.0f)
        return;

    mHP = std::max(0, mHP - dmg);
    mInvincibleTimer = mInvincibleDuration;

    std::cout << "Player took damage! HP: " << mHP << std::endl;

    if (mHP <= 0)
    {
        mIsAlive = false;
        std::cout << "Player has died." << std::endl;
        // Optional: Play explosion or set a death flag
    }
}
