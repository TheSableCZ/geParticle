#pragma once

#include <memory>
#include <geParticle/Particle.h>
#include <geParticle/AoSParticleFactory.h>

namespace ge {
	namespace gl {
		class Context;
	}

	namespace particle {
		class ParticleSystem;
		class GPUParticleContainer;
	}

	namespace examples {
		class PSManager {
		public:
			PSManager() = default;

			void initialize();
			void update();

		private:
			std::shared_ptr<ge::particle::ParticleSystem> ps;
			std::shared_ptr<ge::particle::GPUParticleContainer> pc;

			void distributeParticles(std::shared_ptr<ge::particle::GPUParticleContainer> container);
			void printParticles();
		};
	}
}
