// scenemapstub.h
#pragma once
#include "scene.h"

class SceneMapStub : public Scene
{
public:
    bool Initialise(Renderer& renderer) override { return true; }
    void Process(float deltaTime, InputSystem& inputSystem) override {}
    void Draw(Renderer& renderer) override {}
    void DebugDraw() override {}
};