/** @file ParticleEmitter.h
 *  @brief Abstract base class of emitter object.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geCore/Updatable.h>
#include <memory>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {
    	/**
    	 * @brief Abstract base class of emitter object.
    	 */
        class ParticleEmitter
        {
        public:
	        virtual ~ParticleEmitter() = default;

        	/**
        	 * @brief Add (emit) new particles. Init them in storage.
        	 * @param dt Delta time. Time singe last call.
        	 * @param particles Ptr to particle storage.
        	 */
	        virtual void emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) = 0;
        };
    }
}
