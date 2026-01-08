// COMP710 GP Framework
#ifndef GAME_H
#define GAME_H

// Library includes:
#include <vector>
#include "fmod.hpp"

#include "LevelLoader.h"
#include "player.h"  // Add this at the top
#include "particleemitter.h"//
#include <string>
#include "texture.h"   // ✅ Required for Texture*

// Forward declarations:
class Renderer;
class Sprite;
class Scene;
class InputSystem;


class Game
{
	// Member methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();

	void ToggleDebugWindow();

	void AdvanceScene(); 

	void SetWin();  // add in public section
	void SetScene(int index);  // ✅ Add this here
	void ResetGame();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

protected:
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void ProcessFrameCounting(float deltaTime);

private:
	Game();
	~Game();
	Game(const Game& game);
	Game& operator=(const Game& game);

	// Member data:
public:
	int mScore = 0;
	int mEnemiesKilled = 0;
	float mStageTime = 0.0f;
	float mStageStartTime = 0.0f;//

protected:
	static Game* sm_pInstance;
	Renderer* m_pRenderer;
	InputSystem* m_pInputSystem;

	// Double check these values
	FMOD::System* m_pfmodSystem;
	FMOD::Sound* m_pSwishSound;
	FMOD::Channel* m_pChannel;

	std::vector<Scene*> m_scenes;
	int m_iCurrentScene;

	void DebugDraw();

	__int64 m_iLastTime;
	float m_fExecutionTime;
	float m_fElapsedSeconds;
	int m_iFrameCount;
	int m_iFPS;

	float m_snowTimer = 0.0f;
	bool m_isSnowing = false;
	float m_snowDuration = 0.0f;
	ParticleEmitter* m_snowEmitter = nullptr;//

	bool m_bShowDebugWindow;
	bool m_bPauseSimulation;

	

#ifdef USE_LAG
	float m_fLag;
	int m_iUpdateCount;
#endif // USE_LAG

	bool m_bLooping;
private:

	LevelLoader* mLevel;
	Player* m_pPlayer;  // 👈 Add this

	Sprite* mHeartSprite = nullptr;
	Sprite* mKeySprite = nullptr;
	Sprite* mCoinSprite = nullptr;
	Texture* m_pFloorTexture = nullptr;
	std::string mGuideText;
	float mGuideTimer = 0.0f;
	bool mShowGuide = true;

};

#endif // GAME_H