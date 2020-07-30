/** @file ParticleInitiator.h
 *  @brief Base abstract class for object initializing range of particles.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <memory>
#include <geParticle/ParticleContainerIterator.h>

namespace ge
{
    namespace particle
    {
    	/**
    	 * @brief Base abstract class for object initializing range of particles.
    	 */
		class ParticleInitiator {
		public:
			virtual ~ParticleInitiator() = default;

			/**
			 * @brief Init particles referenced in range particle iterator.
			 * @param range Iterator over particles in storage.
			 */
			virtual void init(std::shared_ptr<RangeParticleContainerIterator> range) = 0;
		};
    }
}
