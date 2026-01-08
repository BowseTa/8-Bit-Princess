//enemy.cpp
#include "enemy.h"
#include "renderer.h"
#include "texture.h"
#include "texturemanager.h"
#include "levelloader.h"
#include "tile.h"
#include <SDL.h>
#include "game.h"

Enemy::Enemy()
    : mSprite(nullptr)
    , mX(0), mY(0)
    , mTileX(0), mTileY(0)
    , mSpeed(64.0f)
    , mDirection(1)
    , mStepTimer(0.0f)
    , mStepInterval(0.4f)
    , mVerticalMove(false)
    , mVerticalDir(1)
    , mLevel(nullptr)
    , mHP(3)
    , mIsAlive(true)
{
}

Enemy::~Enemy()
{
    delete mSprite;
}

bool Enemy::Initialise(Renderer& renderer, int tileX, int tileY, bool vertical)
{
    Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/Enemy.png");
    if (!tex) return false;

    mSprite = new Sprite();
    mSprite->SetScale(64.0f / tex->GetWidth());
    if (!mSprite->Initialise(*tex)) return false;

    mTileX = tileX;
    mTileY = tileY;
    mX = tileX * 64.0f;
    mY = tileY * 64.0f;

    mSprite->SetX(static_cast<int>(mX));
    mSprite->SetY(static_cast<int>(mY));

    mVerticalMove = vertical;
    mDirection = 1;
    mVerticalDir = 1;

    return true;
}

void Enemy::SetLevel(LevelLoader* level)
{
    mLevel = level;
}

void Enemy::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
    mTileX = static_cast<int>(x / 64.0f);
    mTileY = static_cast<int>(y / 64.0f);

    if (mSprite)
    {
        mSprite->SetX(static_cast<int>(mX));
        mSprite->SetY(static_cast<int>(mY));
    }
}

void Enemy::Process(float deltaTime)
{
    if (!mLevel || !mIsAlive) return;

    mStepTimer += deltaTime;
    if (mStepTimer < mStepInterval) return;
    mStepTimer = 0.0f;

    int dx = mVerticalMove ? 0 : mDirection;
    int dy = mVerticalMove ? mVerticalDir : 0;

    int nextX = mTileX + dx;
    int nextY = mTileY + dy;

    SDL_Rect future = { nextX * 64, nextY * 64, 64, 64 };
    bool blocked = false;
    for (Tile* tile : mLevel->GetTiles())
    {
        SDL_Rect box = tile->GetBoundingBox();
        if (tile->IsSolid() && SDL_HasIntersection(&future, &box)) {
            blocked = true;
            break;
        }
    }

    if (!blocked)
    {
        mTileX = nextX;
        mTileY = nextY;
        mX = mTileX * 64.0f;
        mY = mTileY * 64.0f;

        mSprite->SetScaleX((mVerticalMove ? (mVerticalDir == -1 ? -1.0f : 1.0f) : (mDirection == -1 ? -1.0f : 1.0f)));
    }
    else
    {
        if (mVerticalMove)
            mVerticalDir *= -1;
        else
            mDirection *= -1;
    }

    if (mSprite)
    {
        mSprite->SetX(static_cast<int>(mX));
        mSprite->SetY(static_cast<int>(mY));
    }
}

void Enemy::Draw(Renderer& renderer)
{
    if (mSprite && mIsAlive)
    {
        mSprite->Draw(renderer);
    }
}

void Enemy::TakeDamage(int amount)
{
    mHP -= amount;
    if (mHP <= 0)
    {
        mIsAlive = false;
        Game::GetInstance().mScore += 200;
        Game::GetInstance().mEnemiesKilled++;
    }
}

bool Enemy::IsAlive() const
{
    return mIsAlive;
}

SDL_Rect Enemy::GetBoundingBox() const
{
    SDL_Rect box;
    box.x = static_cast<int>(mX);
    box.y = static_cast<int>(mY);
    box.w = 64;
    box.h = 64;
    return box;
}
