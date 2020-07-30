/** @file StandardEmitter.cpp
 *  @brief Base class for all emitters from module geParticleStd. Has lifeTimeInitiator.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleStd/StandardEmitter.h>

ge::particle::StandardEmitter::StandardEmitter(core::time_unit& life, const std::shared_ptr<Counter>& counter)
	: ParticleEmitterBase(counter)
{
	lifeTimeInitiator = std::make_shared<LifeTimeInitiator>(life);
	initiators.push_back(lifeTimeInitiator);
}
