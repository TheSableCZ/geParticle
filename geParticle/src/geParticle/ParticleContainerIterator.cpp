/** @file ParticleContainerIterator.cpp
 *  @brief Classes for iterating through particle container.
 *  @author Jan Sobol xsobol04
 */

#include <geParticle/ParticleContainerIterator.h>
#include <geParticle/ParticleContainer.h>

ge::particle::ParticleContainerType ge::particle::ParticleContainerIterator::getContainerType() const
{
	return container->getType();
}
