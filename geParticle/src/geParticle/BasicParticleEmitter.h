#pragma once

#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleFactory.h>

namespace ge {
	namespace particle {

		class BasicParticleEmitter : public ParticleEmitter
		{
		public:
			BasicParticleEmitter(int particlesPerSecond);

			inline void setParticlesPerSecond(int numOfParticles) { particlesPerSecond = numOfParticles; }

			inline void setParticleFactory(std::shared_ptr<IParticleFactory> factory) { this->factory = factory; }

		protected:
			unsigned int getNumOfParticlesToCreate(core::time_unit dt);

			int particlesPerSecond;
			double carryOver = .0f;

			std::shared_ptr<IParticleFactory> factory;
		};

	}
}
