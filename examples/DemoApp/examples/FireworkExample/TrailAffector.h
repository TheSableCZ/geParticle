#pragma once
#include "TrailEmitter.h"
#include "geParticle/ParticleAffector.h"

namespace ge
{
	namespace examples
	{
		class TrailAffector : public particle::ParticleAffector
		{
		public:
			TrailAffector(std::shared_ptr<TrailEmitter> emitter) : emitter(emitter) {}
			void affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

		protected:
			std::shared_ptr<TrailEmitter> emitter;
			//core::time_unit delay = core::time_unit(0.f);
		};
	}
}
