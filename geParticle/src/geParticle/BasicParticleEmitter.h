#pragma once

#include <geParticle/ParticleEmitter.h>
#include <geParticle/AoSParticleFactory.h>

namespace ge {
	namespace particle {

		class BasicParticleEmitter : public ParticleEmitter
		{
		public:
			BasicParticleEmitter(int particlesPerSecond);

			//explicit BasicParticleEmitter(std::shared_ptr<ParticleSystem> ps);
			//void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;

			inline void setParticlesPerSecond(int numOfParticles) { particlesPerSecond = numOfParticles; }

			inline void setAoSParticleFactory(std::shared_ptr<AoSParticleFactory> aosFactory) { this->aosFactory = aosFactory; }

			//std::unique_ptr<ParticleFactory> particleFactory;

		protected:
			int getNumOfParticlesToCreate(core::time_unit dt);

			//std::shared_ptr<ParticleSystem> particleSystem;
			int particlesPerSecond;
			double carryOver = .0f;

			std::shared_ptr<AoSParticleFactory> aosFactory;
		};

	}
}
