#include <geParticleStd/GravityAffector.h>

#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ArrayOfStructsContainer.h>
#include <geParticle/ComponentSystemContainer.h>

void ge::particle::GravityAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	if (particles->getType() == ParticleContainerType::AoS)
	{
		auto pi = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::iterator>(particles->begin());
		auto end = particles->end();

		for (pi; *pi != *(end); (*pi)++)
		{
			Particle& p = **pi;
			if (p.livingFlag)
			{
				p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt.count();
			}
		}
	}

	if (particles->getType() == ParticleContainerType::SoA_CS)
	{
		auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		auto end = particles->end();

		for (pi; *pi != *(end); (*pi)++)
		{
			auto& v = pi->getComponent<Velocity>();
			v.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt.count();
		}
	}
}
