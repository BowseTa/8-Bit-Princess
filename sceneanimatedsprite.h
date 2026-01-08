#ifndef SCENEANIMATEDSPRITE_H
#define SCENEANIMATEDSPRITE_H

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class Sprite;
class AnimatedSprite;
class InputSystem;

// Class declaration
class SceneAnimatedSprite : public Scene
{
	// Member methods:
public:
	SceneAnimatedSprite();
	virtual ~SceneAnimatedSprite();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw(); // Implementation from the ImGui debugging middleware

protected:

private:
	// Singleton data, ensuring there is only a single instance of this object
	SceneAnimatedSprite(const SceneAnimatedSprite& sceneAnimatedSprite);
	SceneAnimatedSprite& operator=(const SceneAnimatedSprite& sceneAnimatedSprite);

	// Member data:
public:

protected:
	AnimatedSprite* m_pAnimatedImage;

private:

};

#endif // SCENEANIMATEDSPRITE_H