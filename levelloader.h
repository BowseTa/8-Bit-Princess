#pragma once

#include <vector>
#include <SDL.h>
#include <string>

class Renderer;
class Tile;

class LevelLoader
{
public:
    LevelLoader();
    ~LevelLoader();

    bool LoadLevel(const std::string& filename, Renderer& renderer);
    void Draw(Renderer& renderer);

    const std::vector<Tile*>& GetTiles() const;
    const std::vector<SDL_Point>& GetEnemySpawns() const; // ✅ New accessor
    const std::vector<SDL_Point>& GetSlimeSpawns() const;
    const std::vector<SDL_Point>& GetAlphaSpawns() const;
    Tile* GetTile(int col, int row) const;

private:
    void CleanUp();

private:
    std::vector<Tile*> mTiles;
    int mNumRows;
    int mNumCols;
    float mTileSize;

    std::vector<SDL_Point> mEnemySpawns; // ✅ New data member
    std::vector<SDL_Point> mSlimeSpawns;
    std::vector<SDL_Point> mAlphaSpawns;
};


