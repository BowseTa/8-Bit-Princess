#pragma once
#include "scene.h"

class Sprite;
class Renderer;
class InputSystem;

class SceneGameOver : public Scene
{
public:
    SceneGameOver();
    virtual ~SceneGameOver();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
    void DebugDraw() override;

private:
    Sprite* m_pGameOverImage;
    float m_alpha;
};