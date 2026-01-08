// implementation to do, then integrate audio/fmod middleware

// This include:
#include "particleemitter.h"

// Local includes:
#include "renderer.h" 
#include "sprite.h"
#include "particle.h"
#include <iostream>

// Note that these values need to be initialised
ParticleEmitter::ParticleEmitter()
	: m_pSharedSprite(0)
	, m_particles(0)
	, m_fTimeElapsed(0.0f)
	, m_iSpawnBatchSize(0)
	, m_fEmitRate(0.0f)
	, m_fMaxLifespan(0.0f)
	, m_fAccelerationScalar(0.0f)
	, m_fMinAngle(0.0f)
	, m_fMaxAngle(0.0f)
	, m_fX(0.0f)
	, m_fY(0.0f)
{

}

ParticleEmitter::~ParticleEmitter()
{
	delete m_pSharedSprite;

	for (Particle* particle : m_particles)
	{
		delete particle;
	}
}

bool ParticleEmitter::Initialise(Renderer& renderer)
{
	// Subject to change depending on sprite image used for particle emitter
	m_pSharedSprite = renderer.CreateSprite("assets\\Sprites\\snow.png");

	for (Particle* particle : m_particles)
	{
		particle = new Particle();
		particle->Initialise(*m_pSharedSprite);
	}

	return true;
}

void ParticleEmitter::Process(float deltaTime)
{
	// Emit over time — staggered spawning
	m_spawnAccumulator += deltaTime;

	const float spawnInterval = 1.0f / m_fEmitRate;  // e.g., 60 per second

	while (m_spawnAccumulator >= spawnInterval)
	{
		for (int i = 0; i < m_iSpawnBatchSize; ++i)
		{
			Spawn();  // only spawn a few per frame
		}
		m_spawnAccumulator -= spawnInterval;
	}

	// Update particles
	for (auto it = m_particles.begin(); it != m_particles.end(); )
	{
		Particle* particle = *it;

		if (particle->m_bAlive)
		{
			particle->Process(deltaTime);
			++it;
		}
		else
		{
			delete particle;
			it = m_particles.erase(it);
		}
	}
}

void ParticleEmitter::Draw(Renderer& renderer)
{
	// ✅ Add this line right at the top of this function:
	

	for (Particle* particle : m_particles)
	{
		particle->Draw(renderer);
	}
}

void ParticleEmitter::Spawn()
{
	Particle* particle = new Particle();
	particle->Initialise(*m_pSharedSprite);

	particle->m_pSharedSprite->SetScale(0.05f);   // snowflake size
	particle->m_pSharedSprite->SetAlpha(255);     // visible

	particle->m_bAlive = true;
	particle->m_fMaxLifespan = m_fMaxLifespan;
	//particle->m_fCurrentAge = 0.0f;

	// Random X,Y spread from top/right
	// ✅ NEW: Spawn only from the top (across the screen width)
	particle->m_postion.x = rand() % 1920;  // Full map width
	particle->m_postion.y = 0;             // Top edge

	particle->m_velocity.x = -60.0f + (rand() % 20 - 10); // float left
	particle->m_velocity.y = 60.0f + (rand() % 20);       // fall down
	particle->m_acceleration = { 0, 0 };

	particle->m_fColour[0] = m_fColour[0];
	particle->m_fColour[1] = m_fColour[1];
	particle->m_fColour[2] = m_fColour[2];

	m_particles.push_back(particle);
}

void ParticleEmitter::SetSnowMode(float startX, float startY, float velX, float velY, float life, float r, float g, float b)
{
	m_fX = startX;
	m_fY = startY;

	m_fMaxLifespan = life;
	m_fAccelerationScalar = 0.0f;

	m_fColour[0] = r;
	m_fColour[1] = g;
	m_fColour[2] = b;

	m_fEmitRate = 20.0f;           // 60 particles per second
	m_fTimeElapsed = 0.0f;

	m_iSpawnBatchSize = 2;         // Emit 2 particles per frame or tick

	m_spawnAccumulator = 0.0f;      // ✅ Reset spawn timer
	// Clear existing particles
	for (Particle* p : m_particles) delete p;
	m_particles.clear();

	std::cout << "Spawning snow particles!\n";

	// ✅ Spawn some immediately so snow appears right away
	for (int i = 0; i < 10; ++i)
	{
		Spawn();
	}
}