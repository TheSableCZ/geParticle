/** @file LifeTimeInitiator.cpp
 *  @brief Init lifetime attribute.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleStd/LifeTimeInitiator.h>

#include <geParticle/ComponentSystemContainer.h>
#include <geParticle/ArrayOfStructsContainer.h>
#include <geParticleStd/StandardParticleComponents.h>

void ge::particle::LifeTimeInitiator::init(std::shared_ptr<RangeParticleContainerIterator> range)
{
	if (range->getContainerType() == ParticleContainerType::SoA_CS)
	{
		auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			auto& l = begin->getComponent<LifeData>();
			l.livingFlag = true;
			l.life = life;
			l.totalLifeTime = life;
		}
	}
	else if (range->getContainerType() == ParticleContainerType::AoS)
	{
		auto begin = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			auto& particle = **begin;
			particle.livingFlag = true;
			particle.life = life;
			particle.totalLifeTime = life;
		}
	}
}
