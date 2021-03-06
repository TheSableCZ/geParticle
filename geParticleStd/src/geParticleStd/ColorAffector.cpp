﻿/** @file ColorAffector.cpp
 *  @brief Changes color during particle life cycle.
 *  @author Jan Sobol xsobol04
 */

#include "ColorAffector.h"

void ge::particle::ColorAffector::affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles)
{
	if (particles->getType() == ParticleContainerType::SoA_CS)
	{
		auto pi = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<LifeData>();
		auto end = std::static_pointer_cast<ComponentSystemContainer>(particles)->end<LifeData>();

		auto colPi = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<Color>();
		//const auto begin = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		//const auto end = particles->end();

		for (; (*pi) != (*end); ++(*pi), ++(*colPi))
		{
			auto& l = pi->get(); //begin->getComponent<LifeData>();
			auto& c = colPi->get(); //begin->getComponent<Color>();

			if (l.livingFlag)
			{
				auto newCol = getColor(l.life, l.totalLifeTime);
				c.color = newCol;
			}
		}
	}

	// TODO: AoS
}

glm::vec4 ge::particle::ColorAffector::getColor(core::time_unit life, core::time_unit totalLifeTime) const
{
	
	if (colorPoints.empty() || life.count() < 0.f) return glm::vec4(0, 0, 0, 1.f);

	float percentage = 1.f - life.count() / totalLifeTime.count();

	auto it = colorPoints.begin();
	auto last = *it;

	for (; it != colorPoints.end(); ++it)
	{
		if (it->first >= percentage * 100)
			break;

		last = *it;
	}

	auto next = it != colorPoints.end() ? *it : last;

	if ((next.first - last.first) == 0)
		return last.second;

	auto distance = (percentage * 100 - last.first) / (next.first - last.first);

	return last.second + (next.second - last.second) * distance;
}
