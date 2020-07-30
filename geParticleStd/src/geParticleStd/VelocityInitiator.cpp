/** @file VelocityInitiator.cpp
 *  @brief Init velocity attribute. Sphere shooter.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleStd/VelocityInitiator.h>

#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ComponentSystemContainer.h>

float ge::particle::VelocityInitiator::rangeRandom(glm::vec2 range)
{
	auto minimum = range.x;
	auto maximum = range.y;
	return minimum + (maximum - minimum) * rand() / RAND_MAX;
}

void ge::particle::VelocityInitiator::init(std::shared_ptr<RangeParticleContainerIterator> range)
{
	if (range->getContainerType() == ParticleContainerType::SoA_CS)
	{
		auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			float phi = rangeRandom(glm::vec2(0, 2 * PI)); //.get_random();
			float theta = acosf(rangeRandom(glm::vec2(-1, 1)));
			float speed = rangeRandom(speedRange);
			float r = cbrt(rangeRandom(glm::vec2(0, 1)));

			glm::vec3 randomdir = glm::vec3(
				r * sinf(theta) * cosf(phi),
				r * sinf(theta) * sinf(phi),
				r * cosf(theta)
			);

			(*begin).getComponent<Velocity>().velocity = speed * maindir + (randomdir) * spread;
		}
	}
}
