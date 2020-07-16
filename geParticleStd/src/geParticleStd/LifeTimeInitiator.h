#pragma once

#include <geParticle/ParticleInitiator.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ArrayOfStructsContainer.h>

namespace ge {
	namespace particle {
		class LifeTimeInitiator : public ParticleInitiator
		{
		public:
			LifeTimeInitiator(core::time_unit &life)
				: life(life)
			{}

			void init(std::shared_ptr<RangeParticleContainerIterator> range) {
				if (range->getContainerType() == ParticleContainerType::SoA_CS) {
					auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
					auto end = range->end();

					for (; (*begin) != (*end); (*begin)++) {
						auto &l = begin->getComponent<LifeData>();
						l.livingFlag = true;
						l.life = life;
						l.totalLifeTime = life;
					}
				}
				else if (range->getContainerType() == ParticleContainerType::AoS) {
					auto begin = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::range_iterator>(range->begin());
					auto end = range->end();

					for (; (*begin) != (*end); (*begin)++) {
						auto &particle = **begin;
						particle.livingFlag = true;
						particle.life = life;
						particle.totalLifeTime = life;
					}
				}
			}

			void setLife(core::time_unit &life) { this->life = life; }
			core::time_unit& getLife() { return life; }

		protected:
			core::time_unit life;
		};
	}
}
