#include <geParticleStd/BallEmitter.h>

void ge::particle::BallEmitter::BallInitiator::init(std::shared_ptr<RangeParticleContainerIterator> range)
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

glm::vec3 ge::particle::BallEmitter::BallInitiator::getRandPos()
{
	std::uniform_real_distribution<float> uniform01(0.0, 1.0);

	float theta = 2 * PI * uniform01(generator);
	float phi = acos(1 - 2 * uniform01(generator));
	float r = uniform01(generator);
	float x = r * sin(phi) * cos(theta);
	float y = r * sin(phi) * sin(theta);
	float z = r * cos(phi);

	return glm::vec3(x*radius + center.x, y*radius + center.y, z*radius + center.z);
}

ge::particle::BallEmitter::BallEmitter(glm::vec3 center, float radius, core::time_unit& life,
                                       const std::shared_ptr<Counter>& counter)
		: StandardEmitter(life, counter)
{
	initiators.emplace_back(std::make_shared<BallInitiator>(center, radius));
}
