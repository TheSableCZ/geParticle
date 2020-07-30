/** @file ParticleAffector.h
 *  @brief Abstract base class for objects which updating particle attributes during the life cycle.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geCore/Updatable.h>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {
        /**
         * @brief Abstract base class for objects which updating particle attributes during the life cycle.
         */
        class ParticleAffector
        {
        public:
            virtual ~ParticleAffector() = default;

        	/**
        	 * @brief Update particles data. Called in every simulation step.
        	 * @param dt Delta time. Time since last call.
        	 * @param particles Ptr to particle storage. For accessing particles.
        	 */
            virtual void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) = 0;
        };
    }
}
