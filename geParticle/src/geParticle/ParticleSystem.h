/** @file ParticleSystem.h
 *  @brief Particle system class.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geCore/Updatable.h>
#include <memory>
#include <vector>
#include <geParticle/Export.h>

namespace ge
{
	namespace particle
	{
		class ParticleContainer;
	    class ParticleEmitter;
	    class ParticleAffector;

		/**
		 * @brief Particle system class.
		 */
		class GEPARTICLE_EXPORT ParticleSystem : public ge::core::Updatable
		{
		public:
			virtual ~ParticleSystem() = default;
			explicit ParticleSystem(std::shared_ptr<ParticleContainer> container);

			/**
			 * @brief Set start time for first delta time computing. Defines time when simulation starts or continues. Better call with ParticleSystemManager.
			 */
			void setStartTime(const core::time_point& t) { lastTime = t; }

			/**
			 * @brief Propagate update to affectors, emitters. Better call with ParticleSystemManager.
			 */
            void update(const core::time_point& t = core::time_point()) override;

			void addEmitter(std::shared_ptr<ParticleEmitter> emitter);
			void addAffector(std::shared_ptr<ParticleAffector> affector);

			void removeEmitter(std::shared_ptr<ParticleEmitter> emitter);
			void removeAffector(std::shared_ptr<ParticleAffector> affector);

			std::shared_ptr<ParticleContainer> getParticleContainer();

            std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
			std::vector<std::shared_ptr<ParticleAffector>> particleAffectors;

		protected:
			std::shared_ptr<ParticleContainer> particleContainer;

			core::time_point lastTime;
		};
	}
}
