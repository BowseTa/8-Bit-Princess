// Tile.h
#pragma once

#include "renderer.h"

#include "texture.h"  // ✅ Add this line


class Sprite;

enum class TileType { None, Wall, Brick, Coin, Key, Heart, Door };

class Tile
{
public:
    Tile();
    ~Tile();

    bool Initialise(Texture& texture);
    void SetPosition(float x, float y);
    void Draw(Renderer& renderer);

    void Process(float deltaTime); // For spinning coin

    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;

    SDL_Rect GetBoundingBox() const;

    void SetSolid(bool solid) { mIsSolid = solid; }       // ADD
    bool IsSolid() const { return mIsSolid; }             // ADD

    void SetCollectible(bool collectible) { mIsCollectible = collectible; }
    bool IsCollectible() const { return mIsCollectible; }

    void SetCollected(bool collected) { mIsCollected = collected; }
    bool IsCollected() const { return mIsCollected; }

    void SetType(TileType type) { mType = type; }
    TileType GetType() const { return mType; }

private:
    Sprite* mSprite;
    bool mIsSolid = false; // <-- ADD THIS

    bool mIsCollectible = false;
    bool mIsCollected = false;

    float mSpinTime = 0.0f;
    float mSpinSpeed = 1.0f; // much slower // Controls how fast it spins

    TileType mType = TileType::None;
};

