#pragma once

#include <geParticle/ParticleContainer.h>
#include <algorithm>
#include <memory>

namespace ge {
	namespace particle {

		template <class T>
		class SimpleArrayOfStructsContainer : public ArrayOfStructsContainer, public std::enable_shared_from_this<SimpleArrayOfStructsContainer<T>> {

		public:
			class iterator : public IndexBasedParticleContainerIterator
			{
			public:
				iterator(std::shared_ptr<ArrayOfStructsContainer> container)
					: IndexBasedParticleContainerIterator(container, container->startIdx()) {}
				iterator(std::shared_ptr<ArrayOfStructsContainer> container, int idx)
					: IndexBasedParticleContainerIterator(container, idx) {}

				Particle& operator* () const { return std::static_pointer_cast<SimpleArrayOfStructsContainer<T>>(container)->getParticle(idx); }
			};

			class cyclic_iterator : public iterator
			{
			public:
				cyclic_iterator(std::shared_ptr<ArrayOfStructsContainer> container)
					: startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container) {}
				cyclic_iterator(std::shared_ptr<ArrayOfStructsContainer> container, int idx)
					: startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container, idx) {}

				void operator++(int) override { idx++; if (idx == endIdx+1) idx = startIdx; }
				void operator++() override { idx++; if (idx == endIdx+1) idx = startIdx; }

			protected:
				int startIdx;
				int endIdx;
			};

			class filter_iterator : public iterator
			{
			public:
				filter_iterator(std::shared_ptr<ArrayOfStructsContainer> container, bool cyclic = false)
					: cyclic(cyclic), startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container) {}
				filter_iterator(std::shared_ptr<ArrayOfStructsContainer> container, int idx, bool cyclic = false)
					: cyclic(cyclic), startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container, idx) {}

				void setPredicate(std::function<bool(int, std::shared_ptr<ArrayOfStructsContainer>)> predicate) { this->predicate = predicate; }

				void operator++() override {
					int tmpIdx = idx;

					auto csContainer = std::static_pointer_cast<ArrayOfStructsContainer>(container);

					do {
						idx++;
						if (idx == endIdx+1) {
							if (cyclic) idx = startIdx;
							else return;
						}

						if (predicate(idx, csContainer)) return;

						// stop infinite loop when iterator is cyclic
					} while (tmpIdx != idx);
				}
				void operator++(int) override { operator++(); }

			protected:
				bool cyclic;
				int startIdx;
				int endIdx;

				std::function<bool(int, std::shared_ptr<ArrayOfStructsContainer>)> predicate = [](int, std::shared_ptr<ArrayOfStructsContainer>) { return true; };
			};

			class range_iterator : public RangeParticleContainerIterator
			{
			public:
				range_iterator(std::shared_ptr<ArrayOfStructsContainer> container)
					: RangeParticleContainerIterator(container) {}
				range_iterator(std::shared_ptr<ArrayOfStructsContainer> container, std::vector<int> indices, std::vector<int>::iterator pos)
					: RangeParticleContainerIterator(container, indices, pos) {}

				Particle& operator* () const { return std::static_pointer_cast<SimpleArrayOfStructsContainer<T>>(container)->getParticle(*pos); }

				std::shared_ptr<RangeParticleContainerIterator> begin() override {
					return std::make_shared<range_iterator>(
						std::static_pointer_cast<ArrayOfStructsContainer>(container),
						indices,
						indices.begin()
						);
				}

				std::shared_ptr<RangeParticleContainerIterator> end() override {
					return std::make_shared<range_iterator>(
						std::static_pointer_cast<ArrayOfStructsContainer>(container),
						indices,
						indices.end()
						);
				}
			};

		public:
			SimpleArrayOfStructsContainer(int maxParticleCount);

			~SimpleArrayOfStructsContainer();

			Particle & getParticle(int idx) override;
			int startIdx() override;
			int endIdx() override;

			inline unsigned int size() override { return maxParticles; }

			std::shared_ptr<ParticleContainerIterator> begin() override;
			std::shared_ptr<ParticleContainerIterator> end() override;
			std::shared_ptr<ParticleContainerIterator> getUnusedParticlesIterator() override;
			std::shared_ptr<RangeParticleContainerIterator> createRangeIterator() override;

		protected:
			T *particles;
			int maxParticles;
			std::shared_ptr<filter_iterator> unusedParticlesIterator;
		};
	}
}

template <class T>
ge::particle::SimpleArrayOfStructsContainer<T>::SimpleArrayOfStructsContainer(int maxParticleCount)
	: maxParticles(maxParticleCount), ArrayOfStructsContainer()
{
	static_assert(std::is_base_of<Particle, T>::value);
	particles = new T[maxParticleCount];
}

template <class T>
ge::particle::SimpleArrayOfStructsContainer<T>::~SimpleArrayOfStructsContainer()
{
	delete[] particles;
}

template <class T>
ge::particle::Particle & ge::particle::SimpleArrayOfStructsContainer<T>::getParticle(int idx)
{
	assert(idx < maxParticles);
	return particles[idx];
}

template <class T>
int ge::particle::SimpleArrayOfStructsContainer<T>::startIdx()
{
	return 0;
}

template <class T>
int ge::particle::SimpleArrayOfStructsContainer<T>::endIdx()
{
	return maxParticles - 1;
}

template<class T>
inline std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::SimpleArrayOfStructsContainer<T>::begin()
{
	return std::make_shared<iterator>(shared_from_this(), startIdx());
}

template<class T>
inline std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::SimpleArrayOfStructsContainer<T>::end()
{
	return std::make_shared<iterator>(shared_from_this(), size());
}


template<class T>
inline std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::SimpleArrayOfStructsContainer<T>::getUnusedParticlesIterator()
{
	if (!unusedParticlesIterator) {
		unusedParticlesIterator = std::make_shared<filter_iterator>(shared_from_this(), true);
	}
	return unusedParticlesIterator;
}

template<class T>
inline std::shared_ptr<ge::particle::RangeParticleContainerIterator> ge::particle::SimpleArrayOfStructsContainer<T>::createRangeIterator()
{
	return std::make_shared<range_iterator>(shared_from_this());
}
