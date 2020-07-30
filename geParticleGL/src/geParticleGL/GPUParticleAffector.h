/** @file GPUParticleAffector.h
 *  @brief Particle affector accelerated on GPU.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geParticle/ParticleAffector.h>
#include <string>
#include <geParticleGL/ComputeProgramWrapper.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Particle affector accelerated on GPU.
		 */
		class GEPARTICLEGL_EXPORT GPUParticleAffector : public ParticleAffector, public ComputeProgramWrapper {
		public:
			GPUParticleAffector(std::string shaderSource);
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};

	}
}
