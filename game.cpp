// COMP710 GP Framework 2022
// 
// This include:
#include "game.h"

#include "LevelLoader.h"
#include "texturemanager.h"

#include "scenemapstub.h"  // at the top of game.cpp

#include "scenegameover.h"
#include "scenewin.h"
#include "scenestart.h"  // add this

#include "enemy.h"  // ✅ Add this
#include <vector>    // ✅ Add this
#include "slime.h"
#include "alphamonster.h"

// Local library includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "inputsystem.h"
#include "xboxcontroller.h"

// Scene includes
#include "scenecheckerboards.h"
#include "scenebouncingballs.h"
#include "sceneballgame.h"
#include "scenesplashaut.h"
#include "sceneanimatedsprite.h"
#include "fmodsplashscreen.h"

// External library includes
#include "fmod.hpp"
#include "fmod_errors.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

// Static Members:
Game* Game::sm_pInstance = 0;

std::vector<Enemy*> mEnemies;  // ✅ Global vector to store enemy pointers

Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	return (*sm_pInstance);
}

void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game() 
	: m_pRenderer(0)
	, m_pInputSystem(0)
	, m_bLooping(true)
	, m_bPauseSimulation(false)
{

}

Game::~Game()
{
	// Close system and release (free) the sound and system. No need to delete pointer from free store
	

	

	//level
	delete mLevel;
	mLevel = nullptr;

	//player
	delete m_pPlayer;
	m_pPlayer = nullptr;

	// Cleanup enemies
	for (Enemy* enemy : mEnemies)
	{
		delete enemy;
	}
	mEnemies.clear();

	delete m_pRenderer;
	m_pRenderer = 0;

	m_pSwishSound->release();
	m_pfmodSystem->close();
	m_pfmodSystem->release();
}

void Game::Quit()
{
	m_bLooping = false;
}

bool Game::Initialise()
{
	int bbWidth = 820; //840
	int bbHeight = 525; //525

	m_pRenderer = new Renderer();
	m_pInputSystem = new InputSystem();

	if (!m_pRenderer->Initialise(false, bbWidth, bbHeight))
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!");
		return false;
	}

	if (!m_pInputSystem->Initialise())
	{
		LogManager::GetInstance().Log("InputSystem failed to initialise!");
		return false;
	}

	m_pRenderer->SetClearColour(0, 0, 0);
	bbWidth = m_pRenderer->GetWidth();
	bbHeight = m_pRenderer->GetHeight();

	FMOD_RESULT result;
	result = FMOD::System_Create(&m_pfmodSystem);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = m_pfmodSystem->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	m_iLastTime = SDL_GetPerformanceCounter();

	// Create splash scenes

	Scene* pScene0 = new SceneSplashAUT();    // scene 0
	pScene0->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene0);

	Scene* pScene1 = new FmodSplashScreen();  // scene 1
	pScene1->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene1);

	// ✅ Scene 2: Start Screen
	Scene* pScene2 = new SceneStart();       // scene 2
	pScene2->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene2);

	Scene* pScene3 = new SceneMapStub();      // scene 3 (your map)
	pScene3->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene3);

	// ✅ Snow particle system
	m_snowEmitter = new ParticleEmitter();
	m_snowEmitter->Initialise(*m_pRenderer);//

	// Scene 3: Game Over
	Scene* pScene4 = new SceneGameOver();
	pScene4->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene4);

	// Scene 4: Win Scene
	Scene* pScene5 = new SceneWin();
	pScene5->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene5);

	m_iCurrentScene = 0;


	// Load level
	mLevel = new LevelLoader();
	if (!mLevel->LoadLevel("assets/Map/level1.txt", *m_pRenderer))
	{
		LogManager::GetInstance().Log("Failed to load level.");
		return false;
	}

	mHeartSprite = m_pRenderer->CreateSprite("assets/Sprites/heart.png");
	mKeySprite = m_pRenderer->CreateSprite("assets/Sprites/key.png");
	mCoinSprite = m_pRenderer->CreateSprite("assets/Sprites/coin.png");
	m_pFloorTexture = m_pRenderer->GetTextureManager()->GetTexture("assets/Sprites/floor.jpg");

	// Initialise player
	m_pPlayer = new Player();
	if (!m_pPlayer->Initialise(*m_pRenderer))
	{
		return false;
	}
	m_pPlayer->SetLevel(mLevel);

	// ✅ Load enemies from spawn points
	int spawnIndex = 0;
	for (const SDL_Point& spawn : mLevel->GetEnemySpawns())
	{
		Enemy* enemy = new Enemy();

		bool isVertical = (spawnIndex == 1); // make second enemy vertical

		if ((spawn.x == 8 && spawn.y == 5))         // second enemy → vertical
			isVertical = true;
		else if ((spawn.x == 7 && spawn.y == 12))    // eightth enemy → horizontal
			isVertical = false;
		else if ((spawn.x == 1 && spawn.y == 11))    // seventh enemy → horizontal
			isVertical = false;
		else if ((spawn.x == 7 && spawn.y == 10))    // sixth enemy → horizontal
			isVertical = false;
		else if ((spawn.x == 1 && spawn.y == 9))    // fifth enemy → horizontal
			isVertical = false;
		else if ((spawn.x == 8 && spawn.y == 7))    // fourth enemy → horizontal
			isVertical = false;
		else if ((spawn.x == 12 && spawn.y == 1))    // third enemy → horizontal
			isVertical = false;
		else if ((spawn.x == 1 && spawn.y == 5))     // first enemy → horizontal
			isVertical = false;
		if (enemy->Initialise(*m_pRenderer, spawn.x, spawn.y, isVertical))
		{
			enemy->SetLevel(mLevel);
			mEnemies.push_back(enemy);
		}

		spawnIndex++;
	}

	// ✅ Load slime enemies from 'S' tiles
	for (const SDL_Point& pos : mLevel->GetSlimeSpawns())
	{
		Slime* slime = new Slime();
		if (slime->Initialise(*m_pRenderer, pos.x, pos.y, false))  // horizontal movement
		{
			slime->SetLevel(mLevel);
			mEnemies.push_back(slime);  // Store in the same vector
		}
	}

	// ✅ Load alpha monsters from 'A' tiles
	for (const SDL_Point& pos : mLevel->GetAlphaSpawns())
	{
		AlphaMonster* alpha = new AlphaMonster();
		if (alpha->Initialise(*m_pRenderer, pos.x, pos.y, false))  // horizontal by default
		{
			alpha->SetLevel(mLevel);
			mEnemies.push_back(alpha);  // Add to the same enemy list
		}
	}

	// ✅ Initialize score and timing
	mStageStartTime = SDL_GetTicks() / 1000.0f;
	mScore = 0;
	mEnemiesKilled = 0;
	mStageTime = 0.0f;

	// ✅ Add the guide message here
	mGuideText = "Use arrow keys to move.A for attack.\nCollect coins, defeat enemies, collect items and go throught he door!";

	return true;
}

bool Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	// TODO: Process input here!
	m_pInputSystem->ProcessInput();

	if (m_bLooping)
	{
		Uint64 current = SDL_GetPerformanceCounter();
		float deltaTime = (current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());

		m_iLastTime = current;

		m_fExecutionTime += deltaTime;

		Process(deltaTime);

#ifdef USE_LAG
		m_fLag += deltaTime;

		int innerLag = 0;

		while (m_fLag >= stepSize)
		{
			Process(stepSize);

			m_fLag -= stepSize;

			++m_iUpdateCount;
			++innerLag;
		}

#endif //USE_LAG
		Draw(*m_pRenderer);
	}

	return m_bLooping;
}
void Game::Process(float deltaTime)
{
	ProcessFrameCounting(deltaTime);

	// Set deltaTime to zero thereby pausing game
	if (m_bPauseSimulation)
	{
		deltaTime = 0.0f;
	}

	// Update the sound
	m_pfmodSystem->update();


	// Always process input
	m_scenes[m_iCurrentScene]->Process(deltaTime, *m_pInputSystem);

	// Game logic only runs in map scene
	if (m_iCurrentScene == 3)
	{
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (m_pPlayer)
		{
			m_pPlayer->Process(deltaTime, *m_pRenderer, keyState, mEnemies);
		}

		for (Enemy* enemy : mEnemies)
		{
			Slime* slime = dynamic_cast<Slime*>(enemy);
			if (slime)
			{
				slime->Process(deltaTime, m_pPlayer);  // Use simple chase logic
			}
			else
			{
				enemy->Process(deltaTime);  // Normal patrol
			}
		}

		// ✅ Check for player death and switch to Game Over scene
		if (m_pPlayer && !m_pPlayer->IsAlive())
		{
			mStageTime = SDL_GetTicks() / 1000.0f - mStageStartTime;
			m_iCurrentScene = 4;
		}

		// ✅ Only process snow when in the level/map scene
		m_snowTimer += deltaTime;

		if (!m_isSnowing && m_snowTimer >= 10.0f)
		{
			m_isSnowing = true;
			m_snowDuration = 0.0f;

			// Start snow from top-right, move down-left
			m_snowEmitter->SetSnowMode(
				1800.0f, 0.0f,
				-40.0f, 100.0f,
				12.0f,
				1.0f, 1.0f, 1.0f
			);
		}

		if (m_isSnowing)
		{
			m_snowDuration += deltaTime;
			m_snowEmitter->Process(deltaTime);

			if (m_snowDuration >= 12.0f)
			{
				m_isSnowing = false;
				m_snowTimer = 0.0f;
			}
		}

		// ✅ Hide guide text after 10 seconds
		if (mShowGuide)
		{
			mGuideTimer += deltaTime;
			if (mGuideTimer >= 5.0f)
			{
				mShowGuide = false;
			}
		}
	}

	// Later we can use this space to process game input here, based on the studio 6 slides
	// Input process has both keyboard/mouse and xbox controller potential
	
}

void Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;


	// ✅ Set background color based on current scene
	if (m_iCurrentScene == 1)
		renderer.SetClearColour(255, 255, 255); // FMOD, AUT, Start = white
	else if (m_iCurrentScene == 0 || m_iCurrentScene == 2 || m_iCurrentScene == 3)
		renderer.SetClearColour(0, 0, 0); // Map = black
	else if (m_iCurrentScene == 4)
		renderer.SetClearColour(0, 0, 0); // Game Over scene
	else if (m_iCurrentScene == 5)
		renderer.SetClearColour(0, 0, 0); // Win = black background
	// ✅ Clear screen using the selected background color
	renderer.Clear();


	// Play sound (comment out to not play the sound)
	m_pfmodSystem->playSound(m_pSwishSound, 0, false, &m_pChannel);

	

	// TODO: Add game objects to draw here!
	// 
	// Only draw game world if we're in scene 1
	if (m_iCurrentScene == 3)
	{
		// ✅ Draw floor background FIRST
		if (m_pFloorTexture)
		{
			int tileW = m_pFloorTexture->GetWidth();
			int tileH = m_pFloorTexture->GetHeight();

			// Row 1: Top tiles
			{
				// Top-left
				Sprite tileTL;
				tileTL.Initialise(*m_pFloorTexture);
				tileTL.SetX(0);
				tileTL.SetY(0);
				tileTL.SetAlpha(255);
				tileTL.Draw(renderer);

				// Top-right
				Sprite tileTR;
				tileTR.Initialise(*m_pFloorTexture);
				tileTR.SetX(tileW);
				tileTR.SetY(0);
				tileTR.SetAlpha(255);
				tileTR.Draw(renderer);

				// Top-right + 1
				Sprite tileTRPlus;
				tileTRPlus.Initialise(*m_pFloorTexture);
				tileTRPlus.SetX(tileW * 2);
				tileTRPlus.SetY(0);
				tileTRPlus.SetAlpha(255);
				tileTRPlus.Draw(renderer);
			}

			// Row 2: Bottom tiles
			{
				// Bottom-left
				Sprite tileBL;
				tileBL.Initialise(*m_pFloorTexture);
				tileBL.SetX(0);
				tileBL.SetY(tileH);
				tileBL.SetAlpha(255);
				tileBL.Draw(renderer);

				// Bottom-right
				Sprite tileBR;
				tileBR.Initialise(*m_pFloorTexture);
				tileBR.SetX(tileW);
				tileBR.SetY(tileH);
				tileBR.SetAlpha(255);
				tileBR.Draw(renderer);

				// Bottom-right + 1
				Sprite tileBRPlus;
				tileBRPlus.Initialise(*m_pFloorTexture);
				tileBRPlus.SetX(tileW * 2);
				tileBRPlus.SetY(tileH);
				tileBRPlus.SetAlpha(255);
				tileBRPlus.Draw(renderer);
			}
		}

		if (mLevel)
		{
			mLevel->Draw(renderer);
		}

		for (Enemy* enemy : mEnemies)
		{
			enemy->Draw(renderer);
		}

		if (m_pPlayer)
		{
			m_pPlayer->Draw(renderer);
		}
	}

	// Always draw the active scene (e.g., FMOD splash or placeholder)
	m_scenes[m_iCurrentScene]->Draw(renderer);

	// ✅ INSERT HUD HERE (between scene draw and snow/draw/present)
	// ✅ Draw HUD (only in map scene)
	if (m_iCurrentScene == 3 && m_pPlayer)
	{
		int heartSize = 24;
		int spacing = 2;
		int startX = 10;
		int y = 10;

		// ❤️ Draw hearts
		if (mHeartSprite)
		{
			float scale = heartSize / static_cast<float>(mHeartSprite->GetWidth());
			mHeartSprite->SetScale(scale);

			for (int i = 0; i < m_pPlayer->GetHealth(); ++i)
			{
				mHeartSprite->SetX(startX + i * (heartSize + spacing));
				mHeartSprite->SetY(y);
				mHeartSprite->Draw(*m_pRenderer);
			}

			// Reset scale to avoid affecting future draws
			mHeartSprite->SetScale(1.0f);
		}

		// 🔑 Draw key if collected
		if (m_pPlayer->HasKey() && mKeySprite)
		{
			mKeySprite->SetX(10);
			mKeySprite->SetY(y + heartSize + 5); // below hearts
			mKeySprite->SetScale(36.0f / static_cast<float>(mKeySprite->GetWidth()));
			mKeySprite->Draw(*m_pRenderer);

			// Reset scale as well
			mKeySprite->SetScale(1.0f);
		}

		// 🪙 Draw coin icon + count
		if (mCoinSprite)
		{
			float coinSize = 24.0f;
			mCoinSprite->SetScale(coinSize / static_cast<float>(mCoinSprite->GetWidth()));
			mCoinSprite->SetX(10);
			mCoinSprite->SetY(y + heartSize + 30);  // below key
			mCoinSprite->Draw(*m_pRenderer);
			mCoinSprite->SetScale(1.0f);  // reset

			// Draw coin count number using ImGui
			ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
			ImGui::SetWindowPos(ImVec2(15, y + heartSize + 15));  // next to the coin
			ImGui::SetWindowSize(ImVec2(100, 50));
			ImGui::Text("%d", m_pPlayer->GetCoinCount());
			ImGui::End();
		}
	}


	// Debug UI
	DebugDraw();

	// Draw snow ONLY during the map scene
	if (m_iCurrentScene == 3 && m_isSnowing && m_snowEmitter)
	{
		m_snowEmitter->Draw(renderer);
	}

	// ✅ Show guide text
	if (m_iCurrentScene == 3 && mShowGuide && !mGuideText.empty())
	{
		ImGui::Begin("Guide", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
		ImGui::SetWindowPos(ImVec2(400, 300));
		ImGui::SetWindowSize(ImVec2(800, 400));  // optional

		ImGui::SetWindowFontScale(3.0f);  // 🔍 Make text bigger
		ImGui::TextWrapped("%s", mGuideText.c_str());
		ImGui::SetWindowFontScale(1.0f);  // Reset scale (optional)

		ImGui::End();
	}

	// Final presentation
	renderer.Present();

}

void
Game::ProcessFrameCounting(float deltaTime)
{
	// Count total simulation time elapsed:
	m_fElapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_fElapsedSeconds > 1.0f)
	{
		m_fElapsedSeconds -= 1.0f;
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
}

void Game::DebugDraw()
{
	if (m_bShowDebugWindow)
	{
		bool open = true;
		ImGui::Begin("Debug Window", &open, ImGuiWindowFlags_MenuBar);
		ImGui::Text("COMP710 GP Framework (%s)", "2024, S2");

		if (ImGui::Button("Quit"))
		{
			Quit();
		}

		if (ImGui::Button("Pause simulation"))
		{
			m_bPauseSimulation = !m_bPauseSimulation;
		}

		ImGui::SliderInt("Active scene", &m_iCurrentScene, 0, m_scenes.size() - 1, "%d");
		m_scenes[m_iCurrentScene]->DebugDraw();

		// Renderer (textureManager/texture debug draw)
		ImGui::Separator();
		m_pRenderer->DebugDraw();

		// LogManager debug
		ImGui::Separator();
		LogManager::GetInstance().DebugDraw();

		ImGui::End();
	}
}

void Game::ToggleDebugWindow()
{
	m_bShowDebugWindow = !m_bShowDebugWindow;
	m_pInputSystem->ShowMouseCursor(m_bShowDebugWindow);
}

void Game::AdvanceScene()
{
	m_iCurrentScene++;
	if (m_iCurrentScene >= static_cast<int>(m_scenes.size()))
	{
		m_iCurrentScene = static_cast<int>(m_scenes.size()) - 1;
	}
}

void Game::SetWin()
{
	m_iCurrentScene = 5;  // Go to Win Scene
}

void Game::SetScene(int index)
{
	if (index >= 0 && index < static_cast<int>(m_scenes.size()))
	{
		m_iCurrentScene = index;
	}
}

void Game::ResetGame()
{
	// Reset player
	delete m_pPlayer;
	m_pPlayer = new Player();
	if (m_pPlayer->Initialise(*m_pRenderer))
	{
		m_pPlayer->SetLevel(mLevel);
	}

	// Reload level from scratch
	if (mLevel)
	{
		mLevel->LoadLevel("assets/Map/level1.txt", *m_pRenderer);
	}

	m_iCurrentScene = 2;  // Start menu or first gameplay scene

	mScore = 0;
	mEnemiesKilled = 0;
	mStageTime = 0.0f;
	mStageStartTime = SDL_GetTicks() / 1000.0f;
}
