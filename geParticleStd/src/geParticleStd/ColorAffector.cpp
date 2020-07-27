#include "ColorAffector.h"



#include <chrono>
#include <chrono>
#include <chrono>
#include <chrono>
#include <iostream>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>

void ge::particle::ColorAffector::affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles)
{
	if (particles->getType() == ParticleContainerType::SoA_CS)
	{
		const auto begin = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		const auto end = particles->end();

		for (; (*begin) != (*end); (*begin)++)
		{
			auto& l = begin->getComponent<LifeData>();
			auto& c = begin->getComponent<Color>();

			if (l.livingFlag)
			{
				//begin->getComponent<Color>().color = glm::vec3(0.f, 0.f, 0.f);
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
	//std::cout << life.count() << ", " << totalLifeTime.count() << ((life.count() < 0.f) ? "true" : "false") << std::endl;

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

	//std::cout << percentage << "   " << distance << std::endl;

	return last.second + (next.second - last.second) * distance;
}
