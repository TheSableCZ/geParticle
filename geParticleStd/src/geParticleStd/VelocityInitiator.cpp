#include <geParticleStd/VelocityInitiator.h>

#include <geParticleStd/StandardParticleComponents.h>

void ge::particle::VelocityInitiator::init(std::shared_ptr<RangeParticleContainerIterator> range)
{
	if (range->getContainerType() == ParticleContainerType::SoA_CS)
	{
		auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
		auto end = range->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			/*
			//float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);
			// Very bad way to generate a random direction;
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);

			(*begin).getComponent<particle::Velocity>().velocity = maindir + randomdir * spread;
			*/

			/*float theta = rangeRandom(thetaRange); //.get_random();
			float phi = rangeRandom(phiRange);
			float speed = rangeRandom(speedRange);
			*/

			// TODO: standard distribute
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
