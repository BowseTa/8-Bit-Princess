// alphamonster.cpp
#include "alphamonster.h"
#include "texturemanager.h"
#include "sprite.h"

#include "renderer.h"           // ✅ For Renderer
#include "texture.h"            // ✅ For Texture



bool AlphaMonster::Initialise(Renderer& renderer, int tileX, int tileY, bool vertical)
{
    Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/alphamonster.png");
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

    mHP = 5;             // stronger
    mAttackPower = 3;    // hits harder
    mVerticalMove = vertical;
    mDirection = 1;
    mVerticalDir = 1;

    return true;
}
