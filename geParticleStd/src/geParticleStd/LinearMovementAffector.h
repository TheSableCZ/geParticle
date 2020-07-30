/** @file LinearMovementAffector.h
 *  @brief Linear movement affector.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <geParticle/ParticleAffector.h>
#include <geParticleStd/Export.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Linear movement affector.
		 */
		class GEPARTICLESTD_EXPORT LinearMovementAffector : public ParticleAffector {
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};
	}
}
