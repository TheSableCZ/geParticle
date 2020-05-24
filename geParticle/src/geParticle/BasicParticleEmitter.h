#pragma once

#include <geParticle/ParticleEmitter.h>
#include <geParticle/AoSParticleFactory.h>

namespace ge {
	namespace particle {

		class BasicParticleEmitter : public ParticleEmitter
		{
		public:
			BasicParticleEmitter(int particlesPerSecond);

			inline void setParticlesPerSecond(int numOfParticles) { particlesPerSecond = numOfParticles; }

			inline void setAoSParticleFactory(std::shared_ptr<AoSParticleFactory> aosFactory) { this->factory = aosFactory; }

		protected:
			unsigned int getNumOfParticlesToCreate(core::time_unit dt);

			int particlesPerSecond;
			double carryOver = .0f;

			std::shared_ptr<IParticleFactory> factory;
		};

	}
}
