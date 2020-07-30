/** @file ArrayOfStructContainer.h
 *  @brief Simple AoS storage
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geParticle/ParticleContainer.h>
#include <algorithm>
#include <memory>

namespace ge {
	namespace particle {

		class ArrayOfStructsContainerBase : public IndexBasedParticleContainer {
		public:
			virtual Particle & getParticle(int idx) = 0;

			inline ParticleContainerType getType() override { return ParticleContainerType::AoS; }
		};

		/**
		 * @brief Simple AoS particle storage. Particles is stored in an array.
		 */
		template <class T>
		class ArrayOfStructsContainer : public ArrayOfStructsContainerBase, public std::enable_shared_from_this<ArrayOfStructsContainer<T>> {

		public:
			class iterator : public IndexBasedParticleContainerIterator
			{
			public:
				iterator(std::shared_ptr<ArrayOfStructsContainerBase> container)
					: IndexBasedParticleContainerIterator(container, container->startIdx()) {}
				iterator(std::shared_ptr<ArrayOfStructsContainerBase> container, int idx)
					: IndexBasedParticleContainerIterator(container, idx) {}

				Particle& operator* () const { return std::static_pointer_cast<ArrayOfStructsContainer<T>>(container)->getParticle(idx); }
			};

			class cyclic_iterator : public iterator
			{
			public:
				cyclic_iterator(std::shared_ptr<ArrayOfStructsContainerBase> container)
					: startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container) {}
				cyclic_iterator(std::shared_ptr<ArrayOfStructsContainerBase> container, int idx)
					: startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container, idx) {}

				void operator++() override
				{
				    this->idx++;
				    if (this->idx == endIdx+1) this->idx = startIdx;
				}
				void operator++(int) override { operator++(); }

			protected:
				int startIdx;
				int endIdx;
			};

			class filter_iterator : public iterator
			{
			public:
				filter_iterator(std::shared_ptr<ArrayOfStructsContainerBase> container, bool cyclic = false)
					: cyclic(cyclic), startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container) {}
				filter_iterator(std::shared_ptr<ArrayOfStructsContainerBase> container, int idx, bool cyclic = false)
					: cyclic(cyclic), startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container, idx) {}

				void setPredicate(std::function<bool(int, std::shared_ptr<ArrayOfStructsContainerBase>)> predicate) { this->predicate = predicate; }

				void operator++() override {
					int tmpIdx = this->idx;

					auto csContainer = std::static_pointer_cast<ArrayOfStructsContainerBase>(this->container);

					do {
						this->idx++;
						if (this->idx == endIdx+1) {
							if (cyclic) this->idx = startIdx;
							else return;
						}

						if (predicate(this->idx, csContainer)) return;

						// stop infinite loop when iterator is cyclic
					} while (tmpIdx != this->idx);
				}
				void operator++(int) override { operator++(); }

			protected:
				bool cyclic;
				int startIdx;
				int endIdx;

				std::function<bool(int, std::shared_ptr<ArrayOfStructsContainerBase>)> predicate = [](int, std::shared_ptr<ArrayOfStructsContainerBase>) { return true; };
			};

			class range_iterator : public RangeParticleContainerIterator
			{
			public:
				range_iterator(std::shared_ptr<ArrayOfStructsContainerBase> container)
					: RangeParticleContainerIterator(container) {}
				range_iterator(std::shared_ptr<ArrayOfStructsContainerBase> container, std::vector<int> indices, std::vector<int>::iterator pos)
					: RangeParticleContainerIterator(container, indices, pos) {}

				Particle& operator* () const { return std::static_pointer_cast<ArrayOfStructsContainer<T>>(container)->getParticle(*pos); }

				std::shared_ptr<RangeParticleContainerIterator> begin() override {
					return std::make_shared<range_iterator>(
						std::static_pointer_cast<ArrayOfStructsContainerBase>(container),
						indices,
						indices.begin()
						);
				}

				std::shared_ptr<RangeParticleContainerIterator> end() override {
					return std::make_shared<range_iterator>(
						std::static_pointer_cast<ArrayOfStructsContainerBase>(container),
						indices,
						indices.end()
						);
				}
			};

		public:
			ArrayOfStructsContainer(int maxParticleCount);

			~ArrayOfStructsContainer();

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
ge::particle::ArrayOfStructsContainer<T>::ArrayOfStructsContainer(int maxParticleCount)
	: maxParticles(maxParticleCount), ArrayOfStructsContainerBase()
{
	static_assert(std::is_base_of<Particle, T>::value);
	particles = new T[maxParticleCount];
}

template <class T>
ge::particle::ArrayOfStructsContainer<T>::~ArrayOfStructsContainer()
{
	delete[] particles;
}

template <class T>
ge::particle::Particle & ge::particle::ArrayOfStructsContainer<T>::getParticle(int idx)
{
	assert(idx < maxParticles);
	return particles[idx];
}

template <class T>
int ge::particle::ArrayOfStructsContainer<T>::startIdx()
{
	return 0;
}

template <class T>
int ge::particle::ArrayOfStructsContainer<T>::endIdx()
{
	return maxParticles - 1;
}

template<class T>
inline std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::ArrayOfStructsContainer<T>::begin()
{
	return std::make_shared<iterator>(this->shared_from_this(), startIdx());
}

template<class T>
inline std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::ArrayOfStructsContainer<T>::end()
{
	return std::make_shared<iterator>(this->shared_from_this(), size());
}


template<class T>
inline std::shared_ptr<ge::particle::ParticleContainerIterator> ge::particle::ArrayOfStructsContainer<T>::getUnusedParticlesIterator()
{
	if (!unusedParticlesIterator) {
		unusedParticlesIterator = std::make_shared<filter_iterator>(this->shared_from_this(), true);
	}
	return unusedParticlesIterator;
}

template<class T>
inline std::shared_ptr<ge::particle::RangeParticleContainerIterator> ge::particle::ArrayOfStructsContainer<T>::createRangeIterator()
{
	return std::make_shared<range_iterator>(this->shared_from_this());
}
