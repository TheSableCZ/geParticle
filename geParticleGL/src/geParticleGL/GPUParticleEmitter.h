/** @file GPUParticleEmitter.h
 *  @brief Particle emitter accelrated on GPU.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geParticle/ParticleEmitterBase.h>
#include <geParticleGL/ComputeProgramWrapper.h>
#include <geParticleStd/ConstantRateCounter.h>
#include <string>

namespace ge {
	namespace particle {

		/**
		 * @brief Particle emitter accelrated on GPU.
		 */
		class GEPARTICLEGL_EXPORT GPUParticleEmitter : public ParticleEmitterBase, public ComputeProgramWrapper {
		public:
			GPUParticleEmitter(std::string shaderSource, int particlesPerSecond);
			GPUParticleEmitter(std::string shaderSource, std::shared_ptr<Counter> counter);
			void emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

		protected:
			/**
			 * @brief Called from emitParticles. Run compute shader computation.
			 */
			void dispatchEmit(unsigned int newParticlesCount, std::shared_ptr<ParticleContainer> particles);
		};

	}
}
