// COMP710 GP Framework
#ifndef  PARTICLEEMITTER_H
#define  PARTICLEEMITTER_H 

// Library includes:
#include <vector>

// Forward declarations:
class Renderer;
class Sprite;
class Particle;

// Class declaration:
class ParticleEmitter
{
public:  // ✅ This is required!

	// Member methods: public:
	ParticleEmitter();
	~ParticleEmitter();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void Spawn();

	void DebugDraw();

	// ✅ Snow control
	void SetSnowMode(float startX, float startY, float velX, float velY, float life, float r, float g, float b);

protected:

private:
	ParticleEmitter(const ParticleEmitter& particleemitter);
	ParticleEmitter& operator=(const ParticleEmitter& particleemitter);

	// Member data: 
public:

protected:
	Sprite* m_pSharedSprite;
	std::vector<Particle*> m_particles;

	float m_fTimeElapsed;

	int m_iSpawnBatchSize;
	float m_fEmitRate;
	float m_fMaxLifespan;
	float m_fAccelerationScalar;
	float m_fColour[3];
	float m_fMinAngle;
	float m_fMaxAngle;
	float m_fX;
	float m_fY;
	float m_spawnAccumulator;  // ✅ To replace static variable in Process()
private:

};

#endif //  PARTICLEEMITTER_H 
