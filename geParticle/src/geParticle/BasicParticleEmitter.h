#pragma once

#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleFactory.h>
#include <geParticle/Particle.h>
#include <geParticle/ComponentSystemContainer.h>

namespace ge {
	namespace particle {

		class BasicParticleEmitter : public ParticleEmitter
		{
		public:
			BasicParticleEmitter(int particlesPerSecond);

			inline void setParticlesPerSecond(int numOfParticles) { particlesPerSecond = numOfParticles; }

			inline void setParticleFactory(std::shared_ptr<IParticleFactory> factory) { this->factory = factory; }

			void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;

		protected:
			unsigned int getNumOfParticlesToCreate(core::time_unit dt);

			virtual void initAoSParticle(Particle &p) {};
			virtual void initComponentSystemParticle(int idx, std::shared_ptr<ComponentSystemContainer> container) {};

			int particlesPerSecond;
			double carryOver = .0f;

			std::shared_ptr<IParticleFactory> factory;
		};

	}
}
