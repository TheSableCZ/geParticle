#pragma once

#include <geParticle/BasicParticleEmitter.h>
#include <geParticle/ComputeProgramWrapper.h>
#include <string>

namespace ge {
	namespace particle {

		class GPUParticleEmitter : public BasicParticleEmitter, ComputeProgramWrapper {
		public:
			GPUParticleEmitter(std::string shaderSource, int particlesPerSecond);
			void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
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
	program->set("newParticles", getNumOfParticlesToCreate(dt));
	program->set("particleCount", particles->size());
	dispatch(particles->size());
}
