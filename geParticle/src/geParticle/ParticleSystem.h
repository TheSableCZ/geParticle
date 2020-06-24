#pragma once

#include <geCore/Updatable.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace ge
{
	namespace particle
	{
		class ParticleContainer;
	    class ParticleEmitter;
	    class ParticleAffector;

	    class ParticleSystem : public ge::core::Updatable
		{
		public:
			explicit ParticleSystem(std::shared_ptr<ParticleContainer> container);

			void setStartTime(const core::time_point& t) { lastTime = t; }
            void update(const core::time_point& t = core::time_point()) override;

			void addEmitter(std::shared_ptr<ParticleEmitter> emitter);
			void addAffector(std::shared_ptr<ParticleAffector> affector);

			std::shared_ptr<ParticleContainer> getParticleContainer();

            std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
			std::vector<std::shared_ptr<ParticleAffector>> particleAffectors;

		protected:
			std::shared_ptr<ParticleContainer> particleContainer;

			core::time_point lastTime;
		};
	}
}
