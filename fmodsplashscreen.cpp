#include "fmodsplashscreen.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
// Library includes:
#include <cassert>

FmodSplashScreen::FmodSplashScreen()
	: m_pImage(0)
	, m_alpha(0.0f)
	, m_fullyVisible(false)
    , m_skipped(false)
    , m_enterWasPressed(false)
{

}

FmodSplashScreen::~FmodSplashScreen()
{
	delete m_pImage;
}

bool FmodSplashScreen::Initialise(Renderer& renderer)
{
	renderer.SetClearColour(255, 255, 255); // ✅ Set splash background color here

	m_pImage = renderer.CreateSprite("assets\\Sprites\\FMODblack.png");

	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pImage->SetScale(1.0f);
	m_pImage->SetX(SCREEN_WIDTH / 2);
	m_pImage->SetY(SCREEN_HEIGHT / 2);

	m_pImage->SetAlpha(m_alpha);

	return true;
}

void FmodSplashScreen::Process(float deltaTime, InputSystem& inputSystem)
{
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	// Handle Enter key for manual skip
	bool enterPressedNow = keyState[SDL_SCANCODE_RETURN];

	if (enterPressedNow && !m_enterWasPressed && !m_skipped)
	{
		m_skipped = true;
		Game::GetInstance().AdvanceScene(); // Go to next scene
		return;
	}
	m_enterWasPressed = enterPressedNow;

	// If manually skipped, stop all further processing
	if (m_skipped)
	{
		return;
	}

	// Auto fade in/out logic
	if (m_alpha < 1.0f && !m_fullyVisible)
	{
		m_alpha += 0.3f * deltaTime;
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			m_fullyVisible = true;
		}
	}
	else if (m_fullyVisible)
	{
		m_alpha -= 0.3f * deltaTime;
		if (m_alpha <= 0.0f)
		{
			m_alpha = 0.0f;
			Game::GetInstance().AdvanceScene(); // Auto advance
			return;
		}
	}

	// Update image
	if (m_pImage)
	{
		m_pImage->SetAlpha(m_alpha);
		m_pImage->Process(deltaTime);
	}
}

void FmodSplashScreen::Draw(Renderer& renderer)
{
	m_pImage->Draw(renderer);
}

void FmodSplashScreen::DebugDraw()
{

}