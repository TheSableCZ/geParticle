#pragma once
#include <geParticle/ParticleAffector.h>

namespace ge {
	namespace particle {

		class LinearMovementAffector : public ParticleAffector {
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};
	}
}
