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
	program->set("newParticles", newParticlesCount);
	//std::cout << "New particles count: " << getNumOfParticlesToCreate(dt) << std::endl;

	program->set("particleCount", particles->size());
	dispatch(1);
}
