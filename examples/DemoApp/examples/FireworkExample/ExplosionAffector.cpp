/** @file ExplosionAffector.cpp
 *  @brief Affect to initiate explosion.
 *  @author Jan Sobol xsobol04
 */

#include "ExplosionAffector.h"

#include "FireworkExample.h"
#include "geParticle/ComponentSystemContainer.h"
#include "geParticleStd/StandardParticleComponents.h"

void ge::examples::ExplosionAffector::affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles)
{
	auto it = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<particle::LifeData>();
	auto end = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->end<particle::LifeData>();
	//auto it = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->begin());
	//auto end = particles->end();

	auto typePi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<Type>();
	auto explPi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<ExplodedFlag>();

	for (; *it != *end; ++(*it), ++(*typePi), ++(*explPi))
	{
		if (typePi->get().type == 1 && explPi->get().exploded == false && it->get().livingFlag == false)
		{
			explEmitter->addExplosion(
				it->getComponent<particle::Position>().position,
				it->getComponent<particle::Color>().color
			);
			
			explPi->get().exploded = true;
		}
	}
}
