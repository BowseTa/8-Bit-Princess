// COMP710 GP Framework 2022
#ifndef SCENEBALLGAME_H
#define SCENEBALLGAME_H

// Local includes:
#include "scene.h"

// Library includes:
#include <vector>

// Forward declarations:
class Renderer;
class Ball;
class InputSystem;

// Class declaration:
class SceneBallGame : public Scene
{
	// Member methods:
public:
	SceneBallGame();
	virtual ~SceneBallGame();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:
	void SpawnGoodBalls(int number);
	void SpawnBadBalls(int number);

	void CheckCollisions();
	bool BallVsBall(Ball* p1, Ball* p2);

	void MovePlayer(); // Procedure tasked with moving the player ball in accordance with the mouse pointer

private:
	SceneBallGame(const SceneBallGame& sceneballgame);
	SceneBallGame& operator=(const SceneBallGame& sceneballgame);

	// Member data:
public:

protected:
	Renderer* m_pRenderer;
	InputSystem* m_pInputSystem;

	std::vector<Ball*> m_pGoodBalls;
	std::vector<Ball*> m_pBadBalls;

	int m_iNumGoodBalls;
	int m_iNumBadBalls;

	Ball* m_pPlayerBall;

private:

};
#endif // SCENEBALLGAME_H