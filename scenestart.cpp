#include "scenestart.h"
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "imgui/imgui.h"  // ✅ Required for ImGui UI functions

SceneStart::SceneStart()
    : m_pStartImage(nullptr), m_alpha(1.0f) {}

SceneStart::~SceneStart()
{
    delete m_pStartImage;
}

bool SceneStart::Initialise(Renderer& renderer)
{
    m_pStartImage = renderer.CreateSprite("assets/Sprites/start1.png"); // ⬅️ replace with your actual image
    if (!m_pStartImage) return false;

    int screenW = renderer.GetWidth();
    int screenH = renderer.GetHeight();

    // ✅ Scale down the image (try 0.5f or adjust as needed)
    m_pStartImage->SetScale(0.75f);

    m_pStartImage->SetX(screenW / 2);
    m_pStartImage->SetY((screenH / 2) - 100);
    m_pStartImage->SetAlpha(m_alpha);

    return true;
}

void SceneStart::Process(float deltaTime, InputSystem& inputSystem)
{
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    if (keyState[SDL_SCANCODE_RETURN]) // ⬅️ Wait for Enter key
    {
        Game::GetInstance().AdvanceScene();  // ⬅️ Go to map
    }

    m_pStartImage->Process(deltaTime);
}

void SceneStart::Draw(Renderer& renderer)
{
    if (m_pStartImage)
        m_pStartImage->Draw(renderer);

    // ✅ Draw "Press Enter to Start" using ImGui
    ImGui::Begin("StartPrompt", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

    // 🧭 Adjust position: slightly below your sprite
    ImGui::SetWindowPos(ImVec2(500, 600));   // 🔧 Tune these values to match your layout
    ImGui::SetWindowSize(ImVec2(600, 600));  // Box size
    ImGui::SetWindowFontScale(4.0f);         // Make the text larger

    ImGui::Text("Press Enter to Start");

    ImGui::SetWindowFontScale(1.0f);         // Reset for safety
    ImGui::End();
}

void SceneStart::DebugDraw() {}
