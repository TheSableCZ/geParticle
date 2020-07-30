/** @file PointEmitter.cpp
 *  @brief One point emitter.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleStd/PointEmitter.h>

#include <geParticle/ComponentSystemContainer.h>
#include <geParticle/ArrayOfStructsContainer.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticleStd/ConstantRateCounter.h>

void ge::particle::PointEmitter::PointInitiator::init(std::shared_ptr<RangeParticleContainerIterator> range)
{
	if (range->getContainerType() == ParticleContainerType::SoA_CS)
	{
		auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			begin->getComponent<Position>().position = pos;
		}
	}
	else if (range->getContainerType() == ParticleContainerType::AoS)
	{
		auto begin = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			auto& particle = **begin;
			particle.pos = pos;
		}
	}
}

ge::particle::PointEmitter::PointEmitter(int particlesPerSecond, glm::vec3 pos, core::time_unit life): StandardEmitter(
	life, std::make_shared<ConstantRateCounter>(particlesPerSecond))
{
	initiators.push_back(std::make_shared<PointInitiator>(pos));
}
