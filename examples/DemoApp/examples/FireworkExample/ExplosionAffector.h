#pragma once
#include "ExplosionEmitter.h"
#include "geParticle/ParticleAffector.h"

namespace ge
{
	namespace examples
	{
		class ExplosionAffector : public particle::ParticleAffector
		{
		public:
			ExplosionAffector(std::shared_ptr<ExplosionEmitter> explEmitter) : explEmitter(explEmitter) {}
			void affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

		protected:
			std::shared_ptr<ExplosionEmitter> explEmitter;
		};
	}
}
