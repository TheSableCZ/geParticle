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
		auto pi = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<LifeData>();
		auto end = std::static_pointer_cast<ComponentSystemContainer>(particles)->end<LifeData>();

		auto velPi = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<Velocity>();
		//auto velPi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		//auto end = particles->end();

		for (; *pi != *(end); ++(*pi), ++(*velPi))
		{
			if (pi->get().livingFlag) {
				auto& v = velPi->get();
				v.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * static_cast<float>(dt.count());
			}
		}
	}
}
