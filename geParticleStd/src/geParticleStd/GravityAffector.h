/** @file GravityAffector.h
 *  @brief Affector simulating earth gravity.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <geParticle/ParticleAffector.h>
#include <geParticleStd/Export.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Affector simulating earth gravity. 
		 */
		class GEPARTICLESTD_EXPORT GravityAffector : public ParticleAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};
	}
}
