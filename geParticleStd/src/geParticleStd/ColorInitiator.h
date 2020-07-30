/** @file ColorInitiator.h
 *  @brief Init particle color attribute.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <geParticle/ParticleInitiator.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticleStd/Export.h>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Init particle color attribute.
		 */
		class GEPARTICLESTD_EXPORT ColorInitiator : public ParticleInitiator
		{
		public:
			void init(std::shared_ptr<particle::RangeParticleContainerIterator> range) override;

			/**
			 * @brief Init with random color or color in "color" property.
			 */
			bool randomColor = true;

			/**
			 * @brief Color property.
			 */
			glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f);
		};
	}
}
