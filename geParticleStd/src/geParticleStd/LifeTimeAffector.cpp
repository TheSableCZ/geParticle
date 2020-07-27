#include <geParticleStd/LifeTimeAffector.h>

#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ArrayOfStructsContainer.h>
#include <geParticle/ComponentSystemContainer.h>

void ge::particle::LifeTimeAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	if (particles->getType() == ParticleContainerType::AoS)
	{
		auto pi = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::iterator>(particles->begin());
		auto end = particles->end();

		for (pi; *pi != *end; (*pi)++)
		{
			Particle& p = **pi;
			if (p.livingFlag)
			{
				p.life -= dt;
				if (p.life.count() <= 0.0f)
					p.livingFlag = false;
			}
		}
	}

	if (particles->getType() == ParticleContainerType::SoA_CS)
	{
		auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		auto end = particles->end();

		for (pi; *pi != *(end); (*pi)++)
		{
			auto& p = pi->getComponent<LifeData>();
			if (p.livingFlag)
			{
				p.life -= dt;
				if (p.life.count() <= 0.0f)
					p.livingFlag = false;
			}
		}
	}
}
