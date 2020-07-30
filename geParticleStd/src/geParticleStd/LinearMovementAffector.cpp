/** @file LinearMovementAffector.cpp
 *  @brief Linear movement affector.
 *  @author Jan Sobol xsobol04
 */

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
		auto pi = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<LifeData>();
		auto end = std::static_pointer_cast<ComponentSystemContainer>(particles)->end<LifeData>();
		
		auto posIt = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<Position>();
		auto velIt = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<Velocity>();
		//auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		//auto end = particles->end();

		for (; *(pi) != *(end); ++(*pi), ++(*posIt), ++(*velIt))
		{
			if (pi->get().livingFlag) {
				auto& p = posIt->get();
				auto& v = velIt->get();
				p.position += v.velocity * static_cast<float>(dt.count());
			}
		}
	}
}
