#include "slime.h"
#include "renderer.h"
#include "texturemanager.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"
#include "tile.h"  // make sure this is already included
#include "levelloader.h"  // ✅ This gives access to GetTiles()
#include <cstdlib>  // ✅ for rand()

Slime::Slime()
{
}

Slime::~Slime()
{
}

bool Slime::Initialise(Renderer& renderer, int tileX, int tileY, bool vertical)
{
    Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/slime.png");
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

    mHP = 1;
    mIsAlive = true;

    return true;
}


void Slime::Process(float deltaTime, Player* player)
{
    if (!mIsAlive || !player) return;

    mMoveTimer += deltaTime;
    if (mMoveTimer < mMoveInterval) return;
    mMoveTimer = 0.0f;

    int playerTileX = static_cast<int>(player->GetX()) / 64;
    int playerTileY = static_cast<int>(player->GetY()) / 64;

    bool moved = false;

    // Simple axis-aligned chase
    if (mTileX < playerTileX && CanMoveTo(mTileX + 1, mTileY)) { mTileX++; moved = true; }
    else if (mTileX > playerTileX && CanMoveTo(mTileX - 1, mTileY)) { mTileX--; moved = true; }
    else if (mTileY < playerTileY && CanMoveTo(mTileX, mTileY + 1)) { mTileY++; moved = true; }
    else if (mTileY > playerTileY && CanMoveTo(mTileX, mTileY - 1)) { mTileY--; moved = true; }

    // 🌀 If blocked, pick a random direction
    if (!moved)
    {
        int direction = rand() % 4;
        switch (direction)
        {
        case 0: if (CanMoveTo(mTileX + 1, mTileY)) mTileX++; break;  // Right
        case 1: if (CanMoveTo(mTileX - 1, mTileY)) mTileX--; break;  // Left
        case 2: if (CanMoveTo(mTileX, mTileY + 1)) mTileY++; break;  // Down
        case 3: if (CanMoveTo(mTileX, mTileY - 1)) mTileY--; break;  // Up
        }
    }

    // Update screen position
    mX = mTileX * 64.0f;
    mY = mTileY * 64.0f;

    mSprite->SetX(static_cast<int>(mX));
    mSprite->SetY(static_cast<int>(mY));
}

bool Slime::CanMoveTo(int tileX, int tileY)
{
    if (!mLevel) return false;

    SDL_Rect future = { tileX * 64, tileY * 64, 64, 64 };
    for (Tile* tile : mLevel->GetTiles())
    {
        SDL_Rect box = tile->GetBoundingBox();
        if (tile->IsSolid() && SDL_HasIntersection(&future, &box)) {
            return false;  // Collision
        }
    }
    return true;
}