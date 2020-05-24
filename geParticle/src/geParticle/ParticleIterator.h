#pragma once

#include <memory>
#include <geParticle/ParticleContainer.h>

namespace ge {
	namespace particle {
		class ParticleIterator { };

		class IndexBasedParticleIterator : public ParticleIterator {
		public:
			IndexBasedParticleIterator(std::shared_ptr<IndexBasedParticleContainer> container) 
				: container(container), posIdx(container->startIdx()), endIdx(container->endIdx())
			{
			}

			bool end() {
				return (posIdx == (endIdx + 1));
			}

			bool doNext() {
				if (!end()) {
					posIdx++;
					return true;
				}
				else {
					return false;
				}
			}

		protected:
			std::shared_ptr<IndexBasedParticleContainer> container;
			int posIdx;
			int endIdx;
		};

		class AoSParticleIterator : public IndexBasedParticleIterator {
		public:
			AoSParticleIterator(std::shared_ptr<ArrayOfStructsContainer> container)
				: IndexBasedParticleIterator(container)
			{
			}

			Particle & getInstance() {
				return std::static_pointer_cast<ArrayOfStructsContainer>(container)->getParticle(posIdx);
			}
		};
	}
}
