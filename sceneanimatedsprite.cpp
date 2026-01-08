#include "sceneanimatedsprite.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "animatedsprite.h"

// Library includes:
#include <cassert>

SceneAnimatedSprite::SceneAnimatedSprite()
	: m_pAnimatedImage(0)
{
	// Do not need to clear pointer as new object as it is already done in the renderer class
	// i.e. - do not need to do m_pAnimatedImage = new AnimatedSprite();
}

SceneAnimatedSprite::~SceneAnimatedSprite()
{
	delete m_pAnimatedImage;
}

bool SceneAnimatedSprite::Initialise(Renderer& renderer)
{	
	// Currently an issue as calling getwidth or getheight of animatedspirte returns 0
	// Therefore using hardcode values of frame height and width

	// Other assets to try:
	// ..\\assets\\anim8stripx2sheet.png (16 frames, 64x64)
	// ..\\assets\\explosion.png (5 frames, 64x64)

	m_pAnimatedImage = renderer.CreateAnimatedSprite("assets\\Sprites\\explosion.png");
	const int FRAME_WIDTH = 64;
	const int FRAME_HEIGHT = 64;
	m_pAnimatedImage->SetupFrames(FRAME_WIDTH, FRAME_HEIGHT);


	m_pAnimatedImage->SetX(renderer.GetWidth() / 2);
	m_pAnimatedImage->SetY(renderer.GetHeight() / 2);

	m_pAnimatedImage->SetFrameDuration(0.1f);
	m_pAnimatedImage->Animate();
	m_pAnimatedImage->SetLooping(true);

	return true;
}

void SceneAnimatedSprite::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pAnimatedImage->Process(deltaTime);
}

void SceneAnimatedSprite::Draw(Renderer& renderer)
{
	m_pAnimatedImage->Draw(renderer);
}

void SceneAnimatedSprite::DebugDraw()
{

}