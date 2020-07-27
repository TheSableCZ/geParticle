#pragma once

#include <geParticle/ParticleAffector.h>

namespace ge {
	namespace particle {
		class LifeTimeAffector : public ParticleAffector
		{
		public:
			LifeTimeAffector() = default;
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};
	}
}
