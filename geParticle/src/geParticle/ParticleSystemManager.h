/** @file ParticleSystemManager.h
 *  @brief Manager class for instances of ParticleSystem.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <memory>
#include <list>
#include <geCore/Updatable.h>
#include <geParticle/ParticleSystem.h>

namespace ge
{
    namespace particle
    {
    	/**
    	 * @brief Manager class for instances of ParticleSystem.
    	 */
		class GEPARTICLE_EXPORT ParticleSystemManager : public ge::core::Updatable {
		public:
			virtual ~ParticleSystemManager() = default;

			/**
			 * @brief Propagate update message to registered particle systems.
			 */
			void update(const core::time_point& t) override;
			
			void registerParticleSystem(std::shared_ptr<ParticleSystem>& ps, const core::time_point& startTime = core::time_point());
			void unregisterParticleSystem(std::shared_ptr<ParticleSystem>& ps);

			/**
			 * @brief Start simulation. Set time to all particle systems.
			 */
			void startAll(const core::time_point& startTime);

			/**
			 * @brief Pause simulation (animation). When ps manager is paused, update message is not propagated.
			 */
			void pauseAll();

		protected:
			std::list<std::shared_ptr<ParticleSystem>> managed;
			bool paused = true;
		};
    }
}
