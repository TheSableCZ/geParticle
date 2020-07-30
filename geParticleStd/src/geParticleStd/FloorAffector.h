/** @file FloorAffector.h
 *  @brief Affector simulating simple floor.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <geParticle/ParticleAffector.h>
#include <geParticleStd/Export.h>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Affector simulating simple floor.
		 */
		class GEPARTICLESTD_EXPORT FloorAffector : public ParticleAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;

			float axisY = -2;
			float friction = 1.f;
			float resilience = .1f;
			float cutoff = 0.f;
			float floorHitTolerance = 0.1f;
		};
	}
}
