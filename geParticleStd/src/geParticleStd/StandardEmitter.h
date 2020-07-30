/** @file StandardEmitter.h
 *  @brief Base class for all emitters from module geParticleStd. Has lifeTimeInitiator.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <geParticle/ParticleEmitterBase.h>
#include <geParticleStd/LifeTimeInitiator.h>
#include <geParticleStd/Export.h>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Base class for all emitters from module geParticleStd. Has lifeTimeInitiator.
		 */
		class GEPARTICLESTD_EXPORT StandardEmitter : public ParticleEmitterBase
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
