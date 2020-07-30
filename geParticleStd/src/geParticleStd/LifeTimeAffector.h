/** @file LifeTimeAffector.h
 *  @brief Affector for control particles life cycle.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geParticle/ParticleAffector.h>
#include <geParticleStd/Export.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Affector for control particles life cycle.
		 */
		class GEPARTICLESTD_EXPORT LifeTimeAffector : public ParticleAffector
		{
		public:
			LifeTimeAffector() = default;
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};
	}
}
