/** @file ColorAffector.h
 *  @brief Changes color during particle life cycle.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <geParticle/ParticleAffector.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticleStd/Export.h>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Changes color during particle life cycle.
		 */
		class GEPARTICLESTD_EXPORT ColorAffector : public ParticleAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

			/**
			 * @brief Pair of % and color. % gives point inside particle life cycle.
			 */
			std::vector<std::pair<int, glm::vec4>> colorPoints;

		protected:
			glm::vec4 getColor(core::time_unit life, core::time_unit totalLifeTime) const;
		};
	}	
}
