#ifndef SCENESPLASHAUT_H
#define SCENESPLASHAUT_H

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;

// Class declaration
class SceneSplashAUT : public Scene
{
	// Member methods:
public:
	SceneSplashAUT();
	virtual ~SceneSplashAUT();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw(); // Implementation from the ImGui debugging middleware

protected:

private:
	// Singleton data, ensuring there is only a single instance of this object
	SceneSplashAUT(const SceneSplashAUT& sceneCheckerboards);
	SceneSplashAUT& operator=(const SceneSplashAUT& sceneCheckerboards);

	// Member data:
public:

protected:
	Sprite* m_pImage;
	Sprite* m_pText; // Note that static text will be represented as a sprite object

	float m_alpha;
	bool m_fullyVisible;

	bool m_skipped = false;
	bool m_enterWasPressed = false;

private:

};

#endif // SCENESPLASHAUT_H