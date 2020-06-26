#pragma once

#include <geParticle/ParticleAffector.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ArrayOfStructsContainer.h>

namespace ge {
	namespace particle {
		class LifeTimeAffector : public ParticleAffector
		{
		public:
			LifeTimeAffector() = default;
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override
			{
				if (particles->getType() == ParticleContainerType::AoS) {
					auto pi = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::iterator>(particles->begin());
					auto end = particles->end();

					for (pi; *pi != *end; (*pi)++) {
						Particle &p = **pi;
						if (p.livingFlag) {
							p.life -= dt;
							if (p.life.count() <= 0.0f)
								p.livingFlag = false;
						}
					}
				}

				if (particles->getType() == ParticleContainerType::SoA_CS) {
					auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
					auto end = particles->end();

					for (pi; *pi != *(end); (*pi)++) {
						auto &p = pi->getComponent<LifeData>();
						if (p.livingFlag) {
							p.life -= dt;
							if (p.life.count() <= 0.0f)
								p.livingFlag = false;
						}
					}
				}
			}
		};

		class LinearMovementAffector : public ParticleAffector {
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override
			{
				if (particles->getType() == ParticleContainerType::AoS) {
					auto pi = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::iterator>(particles->begin());
					auto end = particles->end();

					for (pi; *pi != *(end); (*pi)++) {
						Particle &p = **pi;
						if (p.livingFlag) {
							p.pos += p.velocity * (float)dt.count();
						}
					}
				}

				if (particles->getType() == ParticleContainerType::SoA_CS) {
					auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
					auto end = particles->end();

					for (pi; *pi != *(end); (*pi)++) {
						auto &p = pi->getComponent<Position>();
						auto &v = pi->getComponent<Velocity>();
						p.position += v.velocity * (float)dt.count();
					}
				}
			}
		};

		class GravityAffector : public ParticleAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override
		    {
				if (particles->getType() == ParticleContainerType::AoS) {
					auto pi = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::iterator>(particles->begin());
					auto end = particles->end();

					for (pi; *pi != *(end); (*pi)++) {
						Particle &p = **pi;
						if (p.livingFlag) {
							p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt.count();
						}
					}
				}

				if (particles->getType() == ParticleContainerType::SoA_CS) {
					auto pi = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
					auto end = particles->end();

					for (pi; *pi != *(end); (*pi)++) {
						auto &v = pi->getComponent<Velocity>();
						v.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt.count();
					}
				}
		    }
		};
	}
}