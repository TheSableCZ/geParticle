#pragma once

#include <geParticle/Particle.h>

namespace ge {
	namespace particle {
		class ParticleContainer { 
		public:
			enum ContainerType {
				AoS, SoA, SoA_CS
			};

			virtual ContainerType getType() = 0;
			virtual unsigned int size() = 0;
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

			inline ContainerType getType() override { return AoS; }
		};

		class StructureOfArraysContainer : public IndexBasedParticleContainer {
			virtual int createParticle() = 0;

			inline ContainerType getType() override { return SoA; }
		};

	}
}
