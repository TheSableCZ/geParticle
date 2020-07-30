/** @file GPUParticleEmitter.cpp
 *  @brief Particle emitter accelrated on GPU.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleGL/GPUParticleEmitter.h>

ge::particle::GPUParticleEmitter::GPUParticleEmitter(std::string shaderSource, int particlesPerSecond)
	: GPUParticleEmitter(shaderSource, std::make_shared<ConstantRateCounter>(particlesPerSecond))
{}

ge::particle::GPUParticleEmitter::GPUParticleEmitter(std::string shaderSource, std::shared_ptr<Counter> counter)
	: ComputeProgramWrapper(shaderSource), ParticleEmitterBase(counter)
{
	checkUniform("newParticles");
	checkUniform("particleCount");
}

void ge::particle::GPUParticleEmitter::emitParticles(core::time_unit dt, std::shared_ptr<ge::particle::ParticleContainer> particles)
{
	unsigned int newParticlesCount = getNumOfParticlesToCreate(dt);
	dispatchEmit(newParticlesCount, particles);
}

void ge::particle::GPUParticleEmitter::dispatchEmit(unsigned int newParticlesCount, std::shared_ptr<ParticleContainer> particles)
{
	program->set("newParticles", newParticlesCount);
	program->set("particleCount", particles->size());
	dispatch(1);
}
