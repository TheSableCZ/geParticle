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
		class ParticleRenderer;

	    class ParticleSystem : public ge::core::Updatable
		{
		public:
			explicit ParticleSystem(std::shared_ptr<ParticleContainer> container);

			void setStartTime(const core::time_point& t) { lastTime = t; }
            void update(const core::time_point& t = core::time_point()) override;

			void addEmitter(std::shared_ptr<ParticleEmitter> emitter);
			void addAffector(std::shared_ptr<ParticleAffector> affector);

            std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
			std::vector<std::shared_ptr<ParticleAffector>> particleAffectors;
			std::shared_ptr<ParticleRenderer> particleRenderer;

		protected:
			std::shared_ptr<ParticleContainer> particleContainer;

			int MaxParticles;
			int LastUsedParticle = 0;

			core::time_point lastTime;
		};
	}
}
