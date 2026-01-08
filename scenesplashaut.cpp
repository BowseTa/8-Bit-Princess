#include "scenesplashaut.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"

#include "game.h"           // ✅ required for AdvanceScene()

// Library includes:
#include <cassert>

SceneSplashAUT::SceneSplashAUT()
	: m_pImage(0)
	, m_pText(0)
	, m_alpha(0.0f)
	, m_fullyVisible(false)
	, m_skipped(false)
	, m_enterWasPressed(false)
{

}

SceneSplashAUT::~SceneSplashAUT()
{
	delete m_pImage;
	delete m_pText;
}

bool SceneSplashAUT::Initialise(Renderer& renderer)
{
	m_pImage = renderer.CreateSprite("assets/Sprites/AUTPicture.jpg");

	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pImage->SetX(SCREEN_WIDTH / 2);
	m_pImage->SetY(SCREEN_HEIGHT / 2);
	m_pImage->SetScale(1.0f);
	m_pImage->SetAlpha(m_alpha);

	m_pText = nullptr;  // ❌ No text used

	return true;
}

void SceneSplashAUT::Process(float deltaTime, InputSystem& inputSystem)
{
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	static bool enterPressedLastFrame = false;
	bool enterPressedNow = keyState[SDL_SCANCODE_RETURN];

	if (enterPressedNow && !enterPressedLastFrame)
	{
		m_skipped = true;
		Game::GetInstance().AdvanceScene(); // Move to start scene
		return;
	}
	enterPressedLastFrame = enterPressedNow;

	if (m_skipped)
		return;

	if (m_alpha < 1.0f && !m_fullyVisible)
	{
		m_alpha += 0.2f * deltaTime;
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			m_fullyVisible = true;
		}
	}
	else if (m_fullyVisible)
	{
		m_alpha -= 0.2f * deltaTime;
		if (m_alpha <= 0.0f)
		{
			m_alpha = 0.0f;
			Game::GetInstance().AdvanceScene(); // Auto-advance only if not skipped
		}
	}

	if (m_pImage)
	{
		m_pImage->SetAlpha(m_alpha);
		m_pImage->Process(deltaTime);
	}
}

void SceneSplashAUT::Draw(Renderer& renderer)
{
	m_pImage->Draw(renderer);
	if (m_pText)
	{
		m_pText->Draw(renderer);
	}
}

void SceneSplashAUT::DebugDraw()
{

}