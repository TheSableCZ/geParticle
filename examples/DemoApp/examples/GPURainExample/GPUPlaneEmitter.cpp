/** @file GPUPlaneEmitter.cpp
 *  @brief GPU emitter with random particle distribution.
 *  @author Jan Sobol xsobol04
 */

#include "GPUPlaneEmitter.h"

ge::examples::GPUPlaneEmitter::GPUPlaneEmitter(std::string shaderSource, int particlesPerSecond, int bufferSize,
	glm::vec2 planeDimensions, float axisY)
		: GPUParticleEmitter(shaderSource, particlesPerSecond),
		bufferSize(bufferSize),
		dimensions(glm::vec2(planeDimensions.x / 2, planeDimensions.y / 2)),
		axisY(axisY),
		generator(core::time_point::clock::now().time_since_epoch().count())
{
	randomBuffer = createBuffer(bufferSize * sizeof(float) * 3, 2);
}

void ge::examples::GPUPlaneEmitter::emitParticles(core::time_unit dt,
	std::shared_ptr<particle::ParticleContainer> particles)
{
	auto newParticlesCount = getNumOfParticlesToCreate(dt);
	refreshRandomBuffer(newParticlesCount);

	dispatchEmit(newParticlesCount, particles);
}

void ge::examples::GPUPlaneEmitter::refreshRandomBuffer(int count)
{
	std::uniform_real_distribution<float> distX(dimensions.x*(-1.f), dimensions.x);
	std::uniform_real_distribution<float> distY(dimensions.y*(-1.f), dimensions.y);

	float *buff = static_cast<float*>(randomBuffer->map());
	
	for (auto i = 0u; i < std::min(count, bufferSize); i++) {
		buff[i*3] = distX(generator);
		buff[i*3 + 1] = axisY;
		buff[i*3 + 2] = distY(generator);
	}

	randomBuffer->unmap();
}
