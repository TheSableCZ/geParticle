#pragma once

#include <geParticle/ParticleAffector.h>
#include <string>
#include <geGL/Program.h>
#include <iostream>
#include <geParticleGL/ComputeProgramWrapper.h>

namespace ge {
	namespace particle {

		class GPUParticleAffector : public ParticleAffector, public ComputeProgramWrapper {
		public:
			GPUParticleAffector(std::string shaderSource);
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};

	}
}

inline ge::particle::GPUParticleAffector::GPUParticleAffector(std::string shaderSource)
	: ComputeProgramWrapper(shaderSource), ParticleAffector()
{
	checkUniform("dt");
	checkUniform("particleCount");
}

void ge::particle::GPUParticleAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	program->set("dt", (float)dt.count());
	program->set("particleCount", particles->size());

	dispatch(particles->size());
}
