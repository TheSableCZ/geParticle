/** @file FloorAffector.cpp
 *  @brief Affector simulating simple floor.
 *  @author Jan Sobol xsobol04
 */

#include "FloorAffector.h"

#include <geParticle/ComponentSystemContainer.h>
#include <geParticleStd/StandardParticleComponents.h>

void ge::particle::FloorAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	if (particles->getType() == ParticleContainerType::SoA_CS) {
		const auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
		const auto end = particles->end();

		for (; *pi != *(end); ++(*pi)) {
			auto &p = pi->getComponent<Position>();
			auto &v = pi->getComponent<Velocity>();

			if (abs(p.position.y - axisY) <= floorHitTolerance)
			{
				p.position.y = axisY;
				//v.velocity = glm::vec3(0, 0, 0);

				glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);
				float nv = glm::dot(normal, v.velocity);

				// Compute tangential and normal components of velocity
				glm::vec3 vn = normal * nv;
				glm::vec3 vt = v.velocity - vn;

				// Compute new velocity
				if (vt.length() * vt.length() <= cutoff) v.velocity = vt - vn * resilience;
				else v.velocity = vt*(1.0f - friction) - vn * resilience;
			}
		}
	}
}
