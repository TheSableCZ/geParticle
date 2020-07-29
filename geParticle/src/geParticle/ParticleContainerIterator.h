#pragma once

#include <memory>
#include <functional>
#include <geParticle/ParticleContainerType.h>

namespace ge {
	namespace particle {

		class ParticleContainer;

		class ParticleContainerIterator
		{
		public:
			virtual ~ParticleContainerIterator() = default;

			ParticleContainerIterator(std::shared_ptr<ParticleContainer> container)
				: container(container) {};

			virtual void operator++(int) = 0;
			virtual void operator++() = 0;
			virtual bool operator==(const ParticleContainerIterator &rhs) const = 0;
			virtual bool operator!=(const ParticleContainerIterator &rhs) const = 0;
			virtual int getIndex() = 0;

			ParticleContainerType getContainerType() const;

		protected:
			std::shared_ptr<ParticleContainer> container;
		};

		class IndexBasedParticleContainerIterator : public ParticleContainerIterator
		{
		public:
			IndexBasedParticleContainerIterator(std::shared_ptr<ParticleContainer> container, int startIdx) 
				: idx(startIdx), ParticleContainerIterator(container)
			{}

			void operator++(int) override { idx++; }
			void operator++() override { idx++; }
			bool operator==(const ParticleContainerIterator &rhs) const override {
				return static_cast<const IndexBasedParticleContainerIterator &>(rhs).idx == idx;
			}
			bool operator!=(const ParticleContainerIterator &rhs) const override {
				return static_cast<const IndexBasedParticleContainerIterator &>(rhs).idx != idx;
			}

			virtual int getIndex() override { return idx; }
			void setIndex(int idx) { this->idx = idx; }

		protected:
			int idx = 0;
		};

		class RangeParticleContainerIterator : public ParticleContainerIterator
		{
		public:
			RangeParticleContainerIterator(std::shared_ptr<ParticleContainer> container)
				: pos(indices.begin()), ParticleContainerIterator(container) {}
			RangeParticleContainerIterator(std::shared_ptr<ParticleContainer> container, std::vector<int> indices, std::vector<int>::iterator pos)
				: indices(indices), pos(pos), ParticleContainerIterator(container) {}

			void operator++(int) override { pos++; }
			void operator++() override { pos++; }
			bool operator==(const ParticleContainerIterator &rhs) const override {
				return static_cast<const RangeParticleContainerIterator &>(rhs).pos == pos;
			}
			bool operator!=(const ParticleContainerIterator &rhs) const override {
				return static_cast<const RangeParticleContainerIterator &>(rhs).pos != pos;
			}

			void addIndex(int idx) {
				indices.push_back(idx);
			}

			virtual void addIndexesFrom(std::shared_ptr<ParticleContainerIterator> iterator, unsigned int count)
			{
				for (unsigned int i = 0; i < count; i++) {
					addIndex(iterator->getIndex());
					++(*iterator);
				}
			}

			virtual std::shared_ptr<RangeParticleContainerIterator> begin() = 0;
			virtual std::shared_ptr<RangeParticleContainerIterator> end() = 0;

			int getIndex() override { return *pos; }

		protected:
			std::vector<int> indices;
			std::vector<int>::iterator pos;
		};
	}
}
