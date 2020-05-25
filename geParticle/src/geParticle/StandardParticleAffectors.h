#pragma once

#include <geParticle/ParticleAffector.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticle/StandardParticleComponents.h>

namespace ge {
	namespace particle {
		class LifeTimeAffector : public ParticleAffector
		{
		public:
			LifeTimeAffector() = default;
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override
			{
				if (particles->getType() == ParticleContainer::AoS) {
					auto pi = AoSParticleIterator(std::static_pointer_cast<ArrayOfStructsContainer>(particles));

					for (pi; !pi.end(); pi.doNext()) {
						Particle &p = pi.getInstance();
						if (p.livingFlag) {
							p.life -= dt;
							if (p.life.count() <= 0.0f)
								p.livingFlag = false;
						}
					}
				}

				if (particles->getType() == ParticleContainer::SoA_CS) {
					auto pi = ComponentSystemParticleIterator(std::static_pointer_cast<ComponentSystemContainer>(particles));

					for (pi; !pi.end(); pi.doNext()) {
						auto &p = pi.getComponent<LifeData>();
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
				if (particles->getType() == ParticleContainer::AoS) {
					auto pi = AoSParticleIterator(std::static_pointer_cast<ArrayOfStructsContainer>(particles));

					for (pi; !pi.end(); pi.doNext()) {
						Particle &p = pi.getInstance();
						if (p.livingFlag) {
							p.pos += p.velocity * (float)dt.count();
						}
					}
				}

				if (particles->getType() == ParticleContainer::SoA_CS) {
					auto pi = ComponentSystemParticleIterator(std::static_pointer_cast<ComponentSystemContainer>(particles));

					for (pi; !pi.end(); pi.doNext()) {
						auto &p = pi.getComponent<Position>();
						auto &v = pi.getComponent<Velocity>();
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
				if (particles->getType() == ParticleContainer::AoS) {
					auto pi = AoSParticleIterator(std::static_pointer_cast<ArrayOfStructsContainer>(particles));

					for (pi; !pi.end(); pi.doNext()) {
						Particle &p = pi.getInstance();
						if (p.livingFlag) {
							p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt.count();
						}
					}
				}

				if (particles->getType() == ParticleContainer::SoA_CS) {
					auto pi = ComponentSystemParticleIterator(std::static_pointer_cast<ComponentSystemContainer>(particles));

					for (pi; !pi.end(); pi.doNext()) {
						auto &v = pi.getComponent<Velocity>();
						v.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt.count();
					}
				}
		    }
		};
	}
}