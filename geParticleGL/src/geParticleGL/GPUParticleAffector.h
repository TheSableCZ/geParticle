#pragma once

#include <geParticle/ParticleAffector.h>
#include <string>
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
