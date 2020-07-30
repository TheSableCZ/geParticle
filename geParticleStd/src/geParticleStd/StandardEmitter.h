#pragma once
#include <geParticle/ParticleEmitterBase.h>
#include <geParticleStd/LifeTimeInitiator.h>

namespace ge
{
	namespace particle
	{
		class StandardEmitter : public ParticleEmitterBase
		{
		public:
			StandardEmitter(core::time_unit& life, const std::shared_ptr<Counter>& counter);

			void setLife(core::time_unit& life) const { lifeTimeInitiator->setLife(life); }
			core::time_unit& getLife() const { return lifeTimeInitiator->getLife(); }
			
		protected:
			std::shared_ptr<LifeTimeInitiator> lifeTimeInitiator;
		};
	}
}
