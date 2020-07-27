#include "BallGPUEmitter.h"

ge::particle::BallGPUEmitter::BallGPUEmitter(std::string shaderSource, int particlesPerSecond, int maxParticles, int ballRadius)
	: ballRadius(ballRadius), GPUParticleEmitter(shaderSource, particlesPerSecond)
{
	randomBuffer = createBuffer(maxParticles * sizeof(float) * 3, 2);
}

void ge::particle::BallGPUEmitter::emitParticles(core::time_unit dt, std::shared_ptr<ge::particle::ParticleContainer> particles)
{
	auto newParticlesCount = getNumOfParticlesToCreate(dt);
	refreshRandomBuffer(newParticlesCount);

	dispatchEmit(newParticlesCount, particles);
}

void ge::particle::BallGPUEmitter::refreshRandomBuffer(int count)
{
	auto fInitRadius = static_cast<float>(ballRadius);
	// TODO: jako private property?
	std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

	float *buff = (float *)randomBuffer->map();
	for (auto i = 0u; i < count * 3; i++) {
		buff[i] = dist(eng);
		//std::cout << buff[i] << std::endl;
	}
	//std::cout << count << " ---------" << std::endl;

	randomBuffer->unmap();
}
