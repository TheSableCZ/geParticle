#pragma once

#include <glm/glm.hpp>
#include <memory>

#include <geParticle/ParticleEmitter.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticle/ArrayOfStructsContainer.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticleStd/ConstantRateCounter.h>
#include <geParticleStd/StandardEmitter.h>

namespace ge {
	namespace particle {

		class PointEmitter : public StandardEmitter
		{
			class PointInitiator : public ParticleInitiator
			{
			public:
				PointInitiator(glm::vec3 pos) : pos(pos) {}

				void init(std::shared_ptr<RangeParticleContainerIterator> range) override
				{
					if (range->getContainerType() == ParticleContainerType::SoA_CS) {
						auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
						auto end = range->end();

						for (; (*begin) != (*end); (*begin)++) {
							begin->getComponent<Position>().position = pos;
						}
					}
					else if (range->getContainerType() == ParticleContainerType::AoS) {
						auto begin = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::range_iterator>(range->begin());
						auto end = range->end();

						for (; (*begin) != (*end); (*begin)++) {
							auto &particle = **begin;
							particle.pos = pos;
						}
					}
				}

			protected:
				glm::vec3 pos;
			};

		public:
			PointEmitter(int particlesPerSecond, glm::vec3 pos, core::time_unit life)
				: StandardEmitter(life, std::make_shared<ConstantRateCounter>(particlesPerSecond))
			{
				initiators.push_back(std::make_shared<PointInitiator>(pos));
			}
		};
	}
}
