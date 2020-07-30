/** @file ParticleRenderer.h
 *  @brief Class which can be used for render particles after simulation step.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <memory>
#include <geParticle/ParticleContainer.h>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Class which can be used for render particles after simulation step.
		 */
		class ParticleRenderer
		{
		public:
			virtual ~ParticleRenderer() = default;

			/**
			 * @brief Render particles.
			 * @param container Particle storage.
			 */
			virtual void render(std::shared_ptr<ParticleContainer> container) = 0;
		};
	}
}
