/** @file BoxEmitter.cpp
 *  @brief Emitter of box shape.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleStd/BoxEmitter.h>

#include <geParticle/ComponentSystemContainer.h>
#include <geParticle/ArrayOfStructsContainer.h>
#include <geParticleStd/StandardParticleComponents.h>

void ge::particle::BoxEmitter::BoxInitiator::init(std::shared_ptr<RangeParticleContainerIterator> range)
{
	if (range->getContainerType() == ParticleContainerType::SoA_CS)
	{
		auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			begin->getComponent<Position>().position = getRandPos();
		}
	}
	else if (range->getContainerType() == ParticleContainerType::AoS)
	{
		auto begin = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			auto& particle = **begin;
			particle.pos = getRandPos();
		}
	}
}

glm::vec3 ge::particle::BoxEmitter::BoxInitiator::getRandPos()
{
	std::uniform_real_distribution<float> xDistribution(x.x, x.y);
	std::uniform_real_distribution<float> yDistribution(y.x, y.y);
	std::uniform_real_distribution<float> zDistribution(z.x, z.y);

	auto result = glm::vec4(xDistribution(generator), yDistribution(generator), zDistribution(generator), 1.f) * rotMat;
	return glm::vec3(result);
}

ge::particle::BoxEmitter::BoxEmitter(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax,
	core::time_unit& life, const std::shared_ptr<Counter>& counter)
		: StandardEmitter(life, counter)
{
	initiators.push_back(std::make_shared<BoxInitiator>(xMin, xMax, yMin, yMax, zMin, zMax));
}
