#pragma once

#include <memory>
#include <geParticle/ParticleContainer.h>

namespace ge {
	namespace particle {
		class ParticleRenderer {
		public:
			virtual void render(std::shared_ptr<ParticleContainer> container) = 0;
		};
	}
}
