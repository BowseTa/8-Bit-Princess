#include "LevelLoader.h"
#include "Tile.h"
#include "Renderer.h"
#include "TextureManager.h"
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem> // Required for current_path()

LevelLoader::LevelLoader()
    : mNumRows(0)
    , mNumCols(0)
    , mTileSize(64.0f)
{
}

LevelLoader::~LevelLoader()
{
    CleanUp();
}

void LevelLoader::CleanUp()
{
    for (Tile* tile : mTiles)
    {
        delete tile;
    }
    mTiles.clear();
    mEnemySpawns.clear();
}

bool LevelLoader::LoadLevel(const std::string& filename, Renderer& renderer)
{
    CleanUp();

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    std::cout << "Trying to open: " << filename << std::endl;

    std::filesystem::path assetPath = std::filesystem::current_path();
    //assetPath /= "game";
    assetPath /= "assets/Map/level1.txt";

    std::cout << "Trying to open: " << assetPath << std::endl;
    std::ifstream file(assetPath);

    if (!file.is_open()) {
        perror("Failed to open level1.txt");
        return false;
    }

    std::string line;
    int row = 0;

    while (std::getline(file, line))
    {
        std::cout << "Line " << row << ": [" << line << "]" << std::endl;
        int col = 0;

        for (char tileChar : line)
        {
            Tile* tile = nullptr;

            if (tileChar == 'W')
            {
                Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/wall.png");
                if (tex)
                {
                    tile = new Tile();
                    tile->Initialise(*tex);
                    tile->SetType(TileType::Wall);
                    tile->SetSolid(true);  // Only wall is solid
                    std::cout << "Loaded WALL at row " << row << ", col " << col << std::endl;
                }
                else
                {
                    std::cout << "Failed to load wall.png for tile at row " << row << ", col " << col << std::endl;
                }
            }
            else if (tileChar == 'B')
            {
                Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/brick1.png");
                if (tex)
                {
                    tile = new Tile();
                    tile->Initialise(*tex);
                    tile->SetType(TileType::Brick);
                    tile->SetSolid(true);  // Only brick is solid
                    std::cout << "Loaded BRICK at row " << row << ", col " << col << std::endl;
                }
                else
                {
                    std::cout << "Failed to load brick.jpg for tile at row " << row << ", col " << col << std::endl;
                }
            }
            else if (tileChar == 'E')
            {
                // Enemy spawn point (invisible tile or floor logic can go here if needed)
                SDL_Point point = { col, row };
                mEnemySpawns.push_back(point);
                std::cout << "Marked ENEMY SPAWN at row " << row << ", col " << col << std::endl;
            }
            else if (tileChar == 'S') // Slime spawn point
            {
                SDL_Point slimePos = { col, row };
                mSlimeSpawns.push_back(slimePos);
                std::cout << "Marked SLIME SPAWN at row " << row << ", col " << col << std::endl;
            }
            else if (tileChar == 'A') // Alpha monster spawn point
            {
                SDL_Point alphaPos = { col, row };
                mAlphaSpawns.push_back(alphaPos);
                std::cout << "Marked ALPHA MONSTER at row " << row << ", col " << col << std::endl;
            }
            else if (tileChar == 'K') // Key
            {
                Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/key.png");
                if (tex)
                {
                    tile = new Tile();
                    tile->Initialise(*tex);
                    tile->SetCollectible(true);
                    tile->SetType(TileType::Key);
                    std::cout << "Loaded KEY at row " << row << ", col " << col << std::endl;
                }
            }
            else if (tileChar == 'C') // Coin
            {
                Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/coin.png");
                if (tex)
                {
                    tile = new Tile();
                    tile->Initialise(*tex);
                    tile->SetCollectible(true);
                    tile->SetType(TileType::Coin);
                    std::cout << "Loaded COIN at row " << row << ", col " << col << std::endl;
                }
            }
            else if (tileChar == 'H') // Heart
            {
                Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/heart.png");
                if (tex)
                {
                    tile = new Tile();
                    tile->Initialise(*tex);
                    tile->SetCollectible(true);
                    tile->SetType(TileType::Heart);
                    std::cout << "Loaded HEART at row " << row << ", col " << col << std::endl;
                }
            }
            else if (tileChar == 'D') // Door
            {
                Texture* tex = renderer.GetTextureManager()->GetTexture("assets/Sprites/door.png");
                if (tex)
                {
                    tile = new Tile();
                    tile->Initialise(*tex);
                    tile->SetType(TileType::Door);
                    std::cout << "Loaded DOOR at row " << row << ", col " << col << std::endl;
                }
            }
            else
            {
                if (tileChar != '\r' && tileChar != '\n')
                {
                    std::cout << "Warning: Unknown tile type '" << tileChar
                        << "' at row " << row << ", col " << col << std::endl;
                }
            }

            if (tile)
            {
                tile->SetPosition(col * mTileSize, row * mTileSize);
                mTiles.push_back(tile);
            }

            ++col;
        }

        ++row;
    }

    mNumRows = row;
    return true;
}

void LevelLoader::Draw(Renderer& renderer)
{
    for (Tile* tile : mTiles)
    {
        tile->Draw(renderer);
    }
}

const std::vector<Tile*>& LevelLoader::GetTiles() const
{
    return mTiles;
}

const std::vector<SDL_Point>& LevelLoader::GetEnemySpawns() const
{
    return mEnemySpawns;
}

const std::vector<SDL_Point>& LevelLoader::GetSlimeSpawns() const
{
    return mSlimeSpawns;
}

const std::vector<SDL_Point>& LevelLoader::GetAlphaSpawns() const
{
    return mAlphaSpawns;
}