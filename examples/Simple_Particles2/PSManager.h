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
	}

	namespace examples {
		class PSManager {
		public:
			PSManager() = default;

			void initialize(std::shared_ptr<ge::gl::Context> glContext);
			void update();

		private:
			std::shared_ptr<ge::particle::ParticleSystem> ps;
		};
	}
}
