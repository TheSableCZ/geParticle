#pragma once

#include <memory>
#include <random>
#include <geParticleStd/StandardParticleComponents.h>

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

			//std::shared_ptr<ge::gl::Buffer> randomBuffer;
			//void refreshRandomBuffer();

			std::vector<ge::particle::GPUMassPointData> distributeParticles();
			void printParticles();

			//std::mt19937 eng;
		};
	}
}
