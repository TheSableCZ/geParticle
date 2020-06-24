#pragma once

#include <memory>
#include <geParticle/Particle.h>
//#include <geParticle/AoSParticleFactory.h>

namespace ge {
	namespace gl {
		class Context;
	}

	namespace particle {
		class ParticleSystem;
		class ComponentSystemRenderer;
		class GPUParticleContainer;
	}

	namespace examples {
		class ComponentSystemPSManager {
		public:
			ComponentSystemPSManager() = default;

			void initialize(std::shared_ptr<ge::gl::Context> glContext);
			void update();

		private:
			std::shared_ptr<ge::particle::ParticleSystem> ps;
			std::shared_ptr<ge::particle::GPUParticleContainer> pc;
			std::shared_ptr<ge::particle::ComponentSystemRenderer> renderer;
		};
	}
}
