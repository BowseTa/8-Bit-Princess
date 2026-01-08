// Local includes:
#include "sceneballgame.h"
#include "renderer.h"
#include "sprite.h"
#include "ball.h"
#include "inputsystem.h"
#include "vector2.h"
#include "imgui/imgui.h"

// Library includes:
#include <cassert>
#include <cmath>

using namespace std;

SceneBallGame::SceneBallGame()
	: m_pRenderer(0)
	, m_pInputSystem(0)
	, m_pBadBalls(0)
	, m_pGoodBalls(0)
	, m_pPlayerBall(0)
{

}

SceneBallGame::~SceneBallGame()
{
	delete m_pPlayerBall;
	delete m_pInputSystem;

	for (Ball* goodBall : m_pGoodBalls)
	{
		delete goodBall;
	}

	for (Ball* badBall : m_pBadBalls)
	{
		delete badBall;
	}
}

bool SceneBallGame::Initialise(Renderer& renderer)
{
	m_pRenderer = &renderer;

	// Initialise the input system
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialise();

	m_iNumGoodBalls = 10;
	m_iNumBadBalls = 10;

	// Initialise the player ball
	m_pPlayerBall = new Ball();
	m_pPlayerBall->Initialise(*m_pRenderer);
	m_pPlayerBall->SetAsPlayer();

	// Initialise the good and bad balls
	SpawnGoodBalls(m_iNumGoodBalls);
	SpawnBadBalls(m_iNumBadBalls);

	return true;
}

void SceneBallGame::SpawnGoodBalls(int number)
{
	for (int i = 0; i < number; i++)
	{
		Ball* goodBall = new Ball();

		goodBall->Initialise(*m_pRenderer);
		goodBall->SetGood();

		m_pGoodBalls.push_back(goodBall);
	}
}

void SceneBallGame::SpawnBadBalls(int number)
{
	for (int i = 0; i < number; i++)
	{
		Ball* badBall = new Ball();

		badBall->Initialise(*m_pRenderer);
		badBall->SetBad();

		m_pBadBalls.push_back(badBall);
	}
}

void SceneBallGame::Process(float deltaTime, InputSystem& inputSystem)
{
	// Process input of moving mouse pointer, then move player ball position according to new mouse position
	m_pInputSystem->ProcessInput();
	MovePlayer();

	// Process player ball after movement due to mouse has been processed
	m_pPlayerBall->Process(deltaTime);

	for (Ball* goodBall : m_pGoodBalls)
	{
		goodBall->Process(deltaTime);
	}

	for (Ball* badBall : m_pBadBalls)
	{
		badBall->Process(deltaTime);
	}

	// Check if any balls are colliding with the player ball
	CheckCollisions();
}

void SceneBallGame::Draw(Renderer& renderer)
{
	m_pPlayerBall->Draw(renderer);

	for (Ball* goodBall : m_pGoodBalls)
	{
		goodBall->Draw(renderer);
	}

	for (Ball* badBall : m_pBadBalls)
	{
		badBall->Draw(renderer);
	}
}

void SceneBallGame::CheckCollisions()
{
	// Loop through good balls and check if distance between each compared to player is 0 or less
	for (Ball* goodBall : m_pGoodBalls)
	{
		// returns true if distance less than 0 (collision)
		if (BallVsBall(m_pPlayerBall, goodBall) && goodBall->IsAlive())
		{
			m_pPlayerBall->Enlarge();
			goodBall->Kill();
		}
	}

	// Loop through bad balls and check if distance between each compared to player is 0 or less
	for (Ball* badBall : m_pBadBalls)
	{
		if (BallVsBall(m_pPlayerBall, badBall) && badBall->IsAlive())
		{
			m_pPlayerBall->Shrink();
			badBall->Kill();
		}
	}
}

bool SceneBallGame::BallVsBall(Ball* p1, Ball* p2)
{
	float pr = p1->GetRadius();
	float br = p2->GetRadius();

	float px = p1->GetX();
	float py = p1->GetY();

	float bx = p2->GetX();
	float by = p2->GetY();

	float distance = sqrt(pow(px - bx, 2) + pow(py - by, 2)) - pr - br;

	// If balls are colliding then return true
	if (distance <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SceneBallGame::MovePlayer()
{
	Vector2 mousePos = m_pInputSystem->GetMousePosition();

	m_pPlayerBall->SetX(mousePos.x);
	m_pPlayerBall->SetY(mousePos.y);
}

void SceneBallGame::DebugDraw()
{
	// Similar to using the std library but this is using the ImGui library from a local include
	ImGui::Text("Scene Ball Game");
	
	ImGui::SliderInt("Show Good Ball Count", &m_iNumGoodBalls, 1, 100);
	ImGui::SliderInt("Show Bad Ball Count", &m_iNumBadBalls, 1, 100);
	
	static int editBallNumber = 0;
	ImGui::SliderInt("Edit ball", &editBallNumber, 0, 99);

	m_pGoodBalls[editBallNumber]->DebugDraw();
	m_pBadBalls[editBallNumber]->DebugDraw();
}