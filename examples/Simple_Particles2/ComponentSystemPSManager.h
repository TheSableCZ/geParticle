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
		class ParticleContainer;
		class ComponentSystemRenderer;
		class GPUParticleContainer;
	}

	namespace examples {
		class ComponentSystemPSManager {
		public:
			ComponentSystemPSManager() = default;

			void initialize(std::shared_ptr<ge::gl::Context> glContext);
			void update();

			inline std::shared_ptr<ge::particle::ParticleSystem> getPs() { return ps; }
			inline std::shared_ptr<ge::particle::ParticleContainer> getPc() { return std::static_pointer_cast<ge::particle::ParticleContainer>(pc); }
			inline std::shared_ptr<ge::particle::ComponentSystemRenderer> getRenderer() { return renderer; }

		private:
			std::shared_ptr<ge::particle::ParticleSystem> ps;
			std::shared_ptr<ge::particle::GPUParticleContainer> pc;
			std::shared_ptr<ge::particle::ComponentSystemRenderer> renderer;
		};
	}
}
