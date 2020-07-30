/** @file ColorInitiator.cpp
 *  @brief Init particle color attribute.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleStd/ColorInitiator.h>

void ge::particle::ColorInitiator::init(std::shared_ptr<particle::RangeParticleContainerIterator> range)
{
	if (range->getContainerType() == particle::ParticleContainerType::SoA_CS)
	{
		auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			//begin->getComponent<Color>().color = glm::vec3(1.f, 0.f, 0.f);
			auto& c = begin->getComponent<Color>().color;
			if (randomColor)
				c = glm::vec4(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)),
				              1.f);
			else
				c = color;
		}
	}

	if (range->getContainerType() == ParticleContainerType::AoS)
	{
		throw std::runtime_error("Default AoS particle does not have color attribute");
	}
}
