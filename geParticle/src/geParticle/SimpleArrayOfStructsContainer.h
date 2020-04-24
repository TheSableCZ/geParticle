#pragma once

#include <geParticle/ParticleContainer.h>

namespace ge {
	namespace particle {
		class SimpleArrayOfStructsContainer : public ArrayOfStructsContainer {
		public:
			explicit SimpleArrayOfStructsContainer(int maxParticleCount);
			~SimpleArrayOfStructsContainer();
			Particle & createParticle() override;
			Particle & getParticle(int idx) override;
			int startIdx() override;
			int endIdx() override;
		protected:
			Particle *particles;

			int findUnusedParticle();
			int maxParticles;
			int lastUsedParticle;
		};
	}
}
