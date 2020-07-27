#include <geParticleStd/LinearMovementAffector.h>

#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ArrayOfStructsContainer.h>
#include <geParticle/ComponentSystemContainer.h>

void ge::particle::LinearMovementAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
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
				p.pos += p.velocity * (float)dt.count();
			}
		}
	}

	if (particles->getType() == ParticleContainerType::SoA_CS)
	{
		auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		auto end = particles->end();

		for (pi; *pi != *(end); (*pi)++)
		{
			auto& p = pi->getComponent<Position>();
			auto& v = pi->getComponent<Velocity>();
			p.position += v.velocity * (float)dt.count();
		}
	}
}
