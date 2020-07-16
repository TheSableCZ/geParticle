#pragma once
#include <geParticle/ParticleInitiator.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticleStd/StandardParticleComponents.h>

namespace ge
{
	namespace particle
	{
		class ColorInitiator : public ParticleInitiator
		{
		public:
			void init(std::shared_ptr<particle::RangeParticleContainerIterator> range) override {
				if (range->getContainerType() == particle::ParticleContainerType::SoA_CS) {
					auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
					auto end = range->end();

					for (; (*begin) != (*end); (*begin)++) {
						//begin->getComponent<Color>().color = glm::vec3(1.f, 0.f, 0.f);
						auto &c = begin->getComponent<Color>().color;
						if (randomColor)
							c = glm::vec3(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)));
						else
							c = color;
					}
				}
			}

			bool randomColor = true;
			glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);
		};
	}
}
