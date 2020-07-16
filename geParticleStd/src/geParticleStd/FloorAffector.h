#pragma once
#include <geParticle/ParticleAffector.h>

namespace ge
{
	namespace particle
	{
		class FloorAffector : public ParticleAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;

			float axisY = -2;
			float friction = 1.f;
			float resilience = .1f;
			float cutoff = 0.f;
			float floorHitTolerance = 0.1f;
		};
	}
}
