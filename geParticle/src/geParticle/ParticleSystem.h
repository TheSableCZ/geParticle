#pragma once

//#include <geParticle/Particle.h>
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
			//explicit ParticleSystem(int MaxParticles);
			explicit ParticleSystem(std::shared_ptr<ParticleContainer> container);
			//~ParticleSystem();

			void setStartTime(const core::time_point& t) { lastTime = t; }
            void update(const core::time_point& t = core::time_point()) override;

            //void addParticle(Particle p);
			//ge::particle::Particle & createParticle();

			//inline int getLivingParticlesCount() { return livingParticlesCount; }
			//inline Particle * getParticleContainer() { return particleContainer; }

			//float *particlesPositions;

			void addEmitter(std::shared_ptr<ParticleEmitter> emitter);
			void addAffector(std::shared_ptr<ParticleAffector> affector);

            std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
			std::vector<std::shared_ptr<ParticleAffector>> particleAffectors;
			std::shared_ptr<ParticleRenderer> particleRenderer;

		private:
			std::shared_ptr<ParticleContainer> particleContainer;

			//void updateBuffer(int index, Particle &p);

            //int findUnusedParticle();
			int MaxParticles;
			int LastUsedParticle = 0;

			//Particle *particleContainer;
            //int livingParticlesCount;

			core::time_point lastTime;
		};
	}
}
