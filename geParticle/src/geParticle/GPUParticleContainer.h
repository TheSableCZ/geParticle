#pragma once

#include <geParticle/ParticleContainer.h>

namespace ge {
	namespace particle {

		class GPUParticleContainer : public IndexBasedParticleContainer {
			GPUParticleContainer(int maxParticleCount);

			//inline ContainerType getType() const { return SoA; };
		};

	}
}
