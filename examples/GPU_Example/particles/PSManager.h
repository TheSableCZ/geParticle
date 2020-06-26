#pragma once

#include <memory>
#include <random>

namespace ge {
	namespace gl {
		class Context;
	}

	namespace particle {
		class ParticleSystem;
		class GPUParticleContainer;
		class SimpleParticleRenderer;
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
			std::shared_ptr<ge::particle::SimpleParticleRenderer> particleRenderer;

			//std::shared_ptr<ge::gl::Buffer> randomBuffer;
			//void refreshRandomBuffer();

			void distributeParticles(std::shared_ptr<ge::particle::GPUParticleContainer> container);
			void printParticles();

			//std::mt19937 eng;
		};
	}
}
