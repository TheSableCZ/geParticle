/** @file LifeTimeInitiator.h
 *  @brief Init lifetime attribute.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geCore/Updatable.h>
#include <geParticle/ParticleInitiator.h>
#include <geParticleStd/Export.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Init lifetime attribute.
		 */
		class GEPARTICLESTD_EXPORT LifeTimeInitiator : public ParticleInitiator
		{
		public:
			LifeTimeInitiator(core::time_unit &life)
				: life(life)
			{}

			void init(std::shared_ptr<RangeParticleContainerIterator> range);

			void setLife(core::time_unit &life) { this->life = life; }
			core::time_unit& getLife() { return life; }

		protected:
			core::time_unit life;
		};
	}
}
