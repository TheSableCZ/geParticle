﻿/** @file TrailAffector.cpp
 *  @brief Affect to leave trails.
 *  @author Jan Sobol xsobol04
 */

#include "TrailAffector.h"

#include "FireworkExample.h"
#include "geParticle/ComponentSystemContainer.h"
#include "geParticleStd/StandardParticleComponents.h"

void ge::examples::TrailAffector::affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles)
{
	auto it = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<particle::LifeData>();
	auto end = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->end<particle::LifeData>();
	//auto it = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->begin());
	//auto end = particles->end();

	auto typePi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<Type>();

	for (; *it != *end; ++(*it), ++(*typePi))
	{
		if (it->get().livingFlag == true && typePi->get().type == 2)
		{
			emitter->addParticle(
				it->getComponent<particle::Position>().position,
				it->getComponent<particle::Color>().color
			);
		}
	}
}
