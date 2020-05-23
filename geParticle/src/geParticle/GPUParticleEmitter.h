#pragma once

#include <geParticle/BasicParticleEmitter.h>
#include <geParticle/ComputeProgramWrapper.h>
#include <string>

namespace ge {
	namespace particle {

		class GPUParticleEmitter : public BasicParticleEmitter, public ComputeProgramWrapper {
		public:
			GPUParticleEmitter(std::string shaderSource, int particlesPerSecond);
			void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;

		protected:
			void dispatchEmit(unsigned int newParticlesCount, std::shared_ptr<ParticleContainer> particles);
		};

	}
}

inline ge::particle::GPUParticleEmitter::GPUParticleEmitter(std::string shaderSource, int particlesPerSecond)
	: ComputeProgramWrapper(shaderSource), BasicParticleEmitter(particlesPerSecond)
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
