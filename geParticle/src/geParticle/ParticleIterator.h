#pragma once

namespace ge {
	namespace particle {
		class ParticleIterator { };

		class IndexBasedParticleIterator : public ParticleIterator {
		public:
			IndexBasedParticleIterator(std::shared_ptr<IndexBasedParticleContainer> container) {
				this->container = container;
				this->posIdx = container->startIdx();
				this->endIdx = container->endIdx();
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
				: IndexBasedParticleIterator(std::static_pointer_cast<IndexBasedParticleContainer>(container))
			{
				this->container = container;
			}

			Particle & getInstance() {
				return std::static_pointer_cast<ArrayOfStructsContainer>(container)->getParticle(posIdx);
			}
		};
	}
}
