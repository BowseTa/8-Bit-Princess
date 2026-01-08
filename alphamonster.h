// alphamonster.h
#pragma once
#include "enemy.h"

class AlphaMonster : public Enemy {
public:
    bool Initialise(Renderer& renderer, int tileX, int tileY, bool vertical) override;
};