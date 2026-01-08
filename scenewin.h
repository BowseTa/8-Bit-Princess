#pragma once
#include "scene.h"

class Sprite;
class Renderer;
class InputSystem;

class SceneWin : public Scene
{
public:
    SceneWin();
    virtual ~SceneWin();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
    void DebugDraw() override;

private:
    Sprite* m_pWinImage;
    float m_alpha;

    float m_rotationAngle = 0.0f;
    float m_spinTimer = 0.0f;
    float m_spinDuration = 2.0f; // spin for 2 seconds
    float m_spinSpeed = 180.0f;  // degrees per second
    bool m_isSpinning = true;

};
