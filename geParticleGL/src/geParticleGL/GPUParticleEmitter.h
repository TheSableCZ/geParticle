#pragma once

#include <geParticle/ParticleEmitter.h>
#include <geParticleGL/ComputeProgramWrapper.h>
#include <geParticleStd/ConstantRateCounter.h>
#include <string>

namespace ge {
	namespace particle {

		class GPUParticleEmitter : public ParticleEmitterBase, public ComputeProgramWrapper {
		public:
			GPUParticleEmitter(std::string shaderSource, int particlesPerSecond);
			GPUParticleEmitter(std::string shaderSource, std::shared_ptr<Counter> counter);
			void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;

		protected:
			void dispatchEmit(unsigned int newParticlesCount, std::shared_ptr<ParticleContainer> particles);
		};

	}
}

inline ge::particle::GPUParticleEmitter::GPUParticleEmitter(std::string shaderSource, int particlesPerSecond)
	: GPUParticleEmitter(shaderSource, std::make_shared<ConstantRateCounter>(particlesPerSecond))
{}

inline ge::particle::GPUParticleEmitter::GPUParticleEmitter(std::string shaderSource, std::shared_ptr<Counter> counter)
	: ComputeProgramWrapper(shaderSource), ParticleEmitterBase(counter)
{
	checkUniform("newParticles");
	checkUniform("particleCount");
}

inline void ge::particle::GPUParticleEmitter::emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	unsigned int newParticlesCount = getNumOfParticlesToCreate(dt);
	dispatchEmit(newParticlesCount, particles);
}

inline void ge::particle::GPUParticleEmitter::dispatchEmit(unsigned int newParticlesCount, std::shared_ptr<ParticleContainer> particles)
{
	program->set("newParticles", newParticlesCount);
	program->set("particleCount", particles->size());
	dispatch(1);
}
