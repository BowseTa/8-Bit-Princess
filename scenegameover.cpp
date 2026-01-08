#include "scenegameover.h"
#include "sprite.h"
#include "renderer.h"
#include "game.h"

#include "imgui/imgui.h"  // ✅ Add this at the top

SceneGameOver::SceneGameOver()
    : m_pGameOverImage(nullptr)
    , m_alpha(0.0f)
{
}

SceneGameOver::~SceneGameOver()
{
    delete m_pGameOverImage;
}

bool SceneGameOver::Initialise(Renderer& renderer)
{
    m_pGameOverImage = renderer.CreateSprite("assets/Sprites/gameover.png");
    if (!m_pGameOverImage) return false;

    int screenW = renderer.GetWidth();
    int screenH = renderer.GetHeight();

    // ✅ Scale down the image (try 0.5f or adjust as needed)
    m_pGameOverImage->SetScale(1.0f);

    m_pGameOverImage->SetX(screenW / 2);
    m_pGameOverImage->SetY(screenH / 2);
    m_pGameOverImage->SetAlpha(m_alpha);
    return true;
}

void SceneGameOver::Process(float deltaTime, InputSystem& inputSystem)
{

    // ✅ Handle Restart (R) and Quit (Q)
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_Q]) {
        Game::GetInstance().Quit();
        return;
    }

    if (keys[SDL_SCANCODE_R]) {
        Game::GetInstance().ResetGame();  // Scene index 2 = your main menu or start scene
        return;
    }

    if (m_alpha < 1.0f)
    {
        m_alpha += 0.5f * deltaTime;
        m_pGameOverImage->SetAlpha(m_alpha);
    }
}

void SceneGameOver::Draw(Renderer& renderer)
{
    if (m_pGameOverImage)
        m_pGameOverImage->Draw(renderer);


    int screenW = renderer.GetWidth();
    int screenH = renderer.GetHeight();
    // Your win image is 320x320 scaled by 0.5 → approx 160x160 final size
    // But visually you want to center in the middle of the whole image area
    ImVec2 textWindowSize(700, 500);

    ImVec2 centeredPos(
        (screenW - textWindowSize.x) * 0.5f + 150.0f,   // This centers the text window
        (screenH - textWindowSize.y) * 0.5f + 200.0f  // Adjust this to push text down visually
    );

    ImGui::SetNextWindowPos(centeredPos);
    ImGui::SetNextWindowSize(textWindowSize);

    ImGui::Begin("Victory Stats", nullptr,
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |     // ✅ prevent scrollbar
        ImGuiWindowFlags_NoInputs);        // optional: disables mouse interaction

    ImGui::SetWindowFontScale(3.5f);  // ✅ Set font size before rendering text

    ImGui::Text("Score: %d", Game::GetInstance().mScore);
    ImGui::Text("Enemies Killed: %d", Game::GetInstance().mEnemiesKilled);
    ImGui::Text("Time: %.2f sec", Game::GetInstance().mStageTime);

    ImGui::End();

    // ✅ Draw Quit & Restart Instructions
    ImGui::Begin("GameOverHelp", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

    ImGui::SetWindowPos(ImVec2(600, 700));     // Adjust this position to place it below the stats
    ImGui::SetWindowSize(ImVec2(500, 120));    // Size large enough for two lines
    ImGui::SetWindowFontScale(2.2f);           // Large readable text

    ImGui::Text("Press Q to Quit");
    ImGui::Text("Press R to Restart");

    ImGui::SetWindowFontScale(1.0f);           // Reset for safety
    ImGui::End();
}

void SceneGameOver::DebugDraw()
{
}
