#include "Tile.h"
#include "Sprite.h"

#include <SDL.h>//



Tile::Tile()
    : mSprite(nullptr)
{
}

Tile::~Tile()
{
    delete mSprite;
}

bool Tile::Initialise(Texture& texture)
{
    mSprite = new Sprite();

    mSprite->SetScale(64.0f / texture.GetWidth());  // 👈 This ensures every tile is scaled to 64x64

    return mSprite->Initialise(texture);
}

void Tile::SetPosition(float x, float y)
{
    if (mSprite)
    {
        mSprite->SetX(static_cast<int>(x));
        mSprite->SetY(static_cast<int>(y));
    }
}

void Tile::Draw(Renderer& renderer)
{
    if (mIsCollected || !mSprite) return;

    if (mType == TileType::Coin)
    {
        Process(1.0f / 60.0f); // assuming 60 FPS; adjust if needed
    }

    mSprite->Draw(renderer);
}

void Tile::Process(float deltaTime)
{
    mSpinTime += deltaTime * mSpinSpeed;

    // Reduce the amplitude of the wobble (from full flip to gentle squash)
    float wobble = cosf(mSpinTime) * 0.5f + 0.5f;  // range: 0.0 to 1.0
    mSprite->SetScaleX(wobble);
}

float Tile::GetX() const
{
    return mSprite ? static_cast<float>(mSprite->GetX()) : 0.0f;
}

float Tile::GetY() const
{
    return mSprite ? static_cast<float>(mSprite->GetY()) : 0.0f;
}

float Tile::GetWidth() const
{
    return mSprite ? static_cast<float>(mSprite->GetWidth()) : 0.0f;
}

float Tile::GetHeight() const
{
    return mSprite ? static_cast<float>(mSprite->GetHeight()) : 0.0f;
}

// 👇 ADD THIS FUNCTION TO THE END
SDL_Rect Tile::GetBoundingBox() const
{
    SDL_Rect rect;
    rect.x = static_cast<int>(GetX());
    rect.y = static_cast<int>(GetY());
    rect.w = static_cast<int>(GetWidth());
    rect.h = static_cast<int>(GetHeight());
    return rect;
}

