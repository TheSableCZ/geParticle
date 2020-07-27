#include "ExplosionAffector.h"

#include "FireworkExample.h"
#include "geParticle/ComponentSystemContainer.h"
#include "geParticleStd/StandardParticleComponents.h"

void ge::examples::ExplosionAffector::affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles)
{
	auto it = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->begin());
	auto end = particles->end();

	for (; *it != *end; ++(*it))
	{
		if (it->getComponent<Type>().type == 1 && it->getComponent<particle::LifeData>().livingFlag == false && it->getComponent<ExplodedFlag>().exploded == false)
		{
			explEmitter->addExplosion(
				it->getComponent<particle::Position>().position,
				it->getComponent<particle::Color>().color
			);
			
			it->getComponent<ExplodedFlag>().exploded = true;
		}
	}
}
