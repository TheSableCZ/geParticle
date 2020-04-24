#pragma once

#include <geParticle/Particle.h>

namespace ge {
	namespace particle {
		class ParticleContainer { 
		public:
			enum ContainerType {
				AoS, SoA
			};

			virtual ContainerType getType() = 0;
		};

		class IndexBasedParticleContainer : public ParticleContainer {
		public:
			virtual int startIdx() = 0;
			virtual int endIdx() = 0;
		};

		class ArrayOfStructsContainer : public IndexBasedParticleContainer {
		public:
			virtual Particle & createParticle() = 0;
			virtual Particle & getParticle(int idx) = 0;

			inline ContainerType getType() { return AoS; };
		};
	}
}
