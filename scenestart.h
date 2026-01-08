#pragma once
#include "scene.h"
#include "sprite.h"

class SceneStart : public Scene
{
public:
    SceneStart();
    ~SceneStart();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
    void DebugDraw() override;

private:
    Sprite* m_pStartImage;
    float m_alpha;
};