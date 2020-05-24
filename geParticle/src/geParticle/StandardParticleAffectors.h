#pragma once

#include <geParticle/ParticleAffector.h>

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
			}
		};

		//    class GravityAffector : public ParticleAffector
		//    {
		//    public:
		//        void affect(core::time_unit dt, Particle &p) override
		//        {
					////std::cout << "ref: " << p.pos.y + p.velocity.y * (float) dt.count() + 0.5 * 9.81f * (float) dt.count() * (float) dt.count() << "\n";
		//            p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float) dt.count()  /* * 0.5f */;
		//            p.pos += p.velocity * (float) dt.count();
					////std::cout << "res: " << p.pos.y << "\n";
		//        }
		//    };
	}
}