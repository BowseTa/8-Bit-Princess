#ifndef FMODSPLASHSCREEN_H
#define FMODSPLASHSCREEN_H

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;

// Class declaration
class FmodSplashScreen : public Scene
{
	// Member methods:
public:
	FmodSplashScreen();
	virtual ~FmodSplashScreen();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw(); // Implementation from the ImGui debugging middleware

protected:

private:
	// Singleton data, ensuring there is only a single instance of this object
	FmodSplashScreen(const FmodSplashScreen& fmodSplashScreen);
	FmodSplashScreen& operator=(const FmodSplashScreen& fmodSplashScreen);

	// Member data:
public:

protected:
	Sprite* m_pImage;

	float m_alpha;
	bool m_fullyVisible;

	bool m_enterWasPressed = false;
	bool m_skipped = false;  // ✅ properly initialized

private:

};

#endif // FMODSPLASHSCREEN_H