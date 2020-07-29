#pragma once

#include <geParticle/ParticleContainer.h>
#include <unordered_map>
#include <utility>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <set>
#include <geParticle/ComponentPool.h>
#include <geParticle/ParticleContainerIterator.h>

namespace ge {
	namespace particle {

		class ComponentSystemContainer : public IndexBasedParticleContainer, public std::enable_shared_from_this<ComponentSystemContainer> {
		protected:
			using PredicateFunction = std::function<bool(const int, const ComponentSystemContainer &)>;

		public:
			class iterator : public IndexBasedParticleContainerIterator
			{
			public:
				iterator(std::shared_ptr<ComponentSystemContainer> container)
					: IndexBasedParticleContainerIterator(container, container->startIdx()) {}
				iterator(std::shared_ptr<ComponentSystemContainer> container, int idx)
					: IndexBasedParticleContainerIterator(container, idx) {}

				template <typename T>
				T& getComponent() const { return std::static_pointer_cast<ComponentSystemContainer>(container)->getComponent<T>(idx); }

				void startUniqueSelection() { uniqueSel = true; indexSet.clear(); }
				void endUniqueSelection() { uniqueSel = false; }

				int getIndex() override
				{
					if (uniqueSel)
					{
						indexSet.emplace(idx);
					}
					return IndexBasedParticleContainerIterator::getIndex();
				}

			protected:
				bool uniqueSel = false;
				std::set<int> indexSet;
			};

			template <typename T>
			class component_iterator : public iterator
			{
			public:
				component_iterator(std::shared_ptr<ComponentSystemContainer> container)
					: iterator(container)
				{
					component = container->getComponent<T>();
				}
				component_iterator(std::shared_ptr<ComponentSystemContainer> container, int idx)
					: iterator(container, idx)
				{
					component = container->getComponent<T>();
				}
					
				T& get()
				{
					return component->get(idx);
				}

			protected:
				std::shared_ptr<ComponentPool<T>> component;
			};

			class cyclic_iterator : public iterator
			{
			public:
				cyclic_iterator(std::shared_ptr<ComponentSystemContainer> container)
					: startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container) {}
				cyclic_iterator(std::shared_ptr<ComponentSystemContainer> container, int idx)
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
				filter_iterator(std::shared_ptr<ComponentSystemContainer> container, bool cyclic = false)
					: cyclic(cyclic), startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container) {}
				filter_iterator(std::shared_ptr<ComponentSystemContainer> container, int idx, bool cyclic = false)
					: cyclic(cyclic), startIdx(container->startIdx()), endIdx(container->endIdx()), iterator(container, idx) {}

				void setPredicate(PredicateFunction predicate) { this->predicate = std::move(predicate); }

				void operator++() override {
					int tmpIdx = idx;

					auto csContainer = std::static_pointer_cast<ComponentSystemContainer>(container);

					do {
						idx++;
						if (idx == endIdx+1) {
							if (cyclic) idx = startIdx;
							else return;
						}

						if (predicate(idx, *csContainer)) return;

						// stop infinite loop when iterator is cyclic
					} while (tmpIdx != idx);
				}
				void operator++(int) override { operator++(); }

			protected:
				bool cyclic;
				int startIdx;
				int endIdx;

				PredicateFunction predicate = [](const int, const ComponentSystemContainer &) { return true; };
			};

			class unused_particles_iterator : public filter_iterator
			{
			public:
				unused_particles_iterator(std::shared_ptr<ComponentSystemContainer> container, bool cyclic = false)
					: filter_iterator(std::move(container), cyclic) {}
				unused_particles_iterator(std::shared_ptr<ComponentSystemContainer> container, int idx, bool cyclic = false)
					: filter_iterator(std::move(container), idx, cyclic) {}
				void operator++() override
				{
					int tmpIdx = idx;

					auto csContainer = std::static_pointer_cast<ComponentSystemContainer>(container);

					do {
						idx++;
						if (idx == endIdx + 1) {
							if (cyclic) idx = startIdx;
							else return;
						}

						if (predicate(idx, *csContainer) && indexSet.find(idx) == indexSet.end()) return;

						// stop infinite loop when iterator is cyclic
					} while (tmpIdx != idx);

					if (tmpIdx == idx)
					{
						// unused particle not found
						do {
							idx = (rand() % container->size()) + startIdx;
						} while (indexSet.find(idx) != indexSet.end());
					}
				}
			};

			class range_iterator : public RangeParticleContainerIterator
			{
			public:
				range_iterator(std::shared_ptr<ComponentSystemContainer> container)
					: RangeParticleContainerIterator(container) {}
				range_iterator(std::shared_ptr<ComponentSystemContainer> container, std::vector<int> indices, std::vector<int>::iterator pos)
					: RangeParticleContainerIterator(container, indices, pos) {}

				template <typename T>
				T& getComponent() const { return std::static_pointer_cast<ComponentSystemContainer>(container)->getComponent<T>(*pos); }

				void addIndexesFrom(std::shared_ptr<ParticleContainerIterator> iterator, unsigned count) override
				{
					if (iterator->getContainerType() == ParticleContainerType::SoA_CS)
					{
						auto csIterator = std::static_pointer_cast<ComponentSystemContainer::iterator>(iterator);
						csIterator->startUniqueSelection();
						for (unsigned int i = 0; i < count; i++) {
							addIndex(csIterator->getIndex());
							++(*csIterator);
						}
						csIterator->endUniqueSelection();
					}
					else
					{
						RangeParticleContainerIterator::addIndexesFrom(iterator, count);
					}
				}

				std::shared_ptr<RangeParticleContainerIterator> begin() override {
					return std::make_shared<range_iterator>(
						std::static_pointer_cast<ComponentSystemContainer>(container),
						indices,
						indices.begin()
						);
				}

				std::shared_ptr<RangeParticleContainerIterator> end() override {
					return std::make_shared<range_iterator>(
						std::static_pointer_cast<ComponentSystemContainer>(container),
						indices,
						indices.end()
						);
				}
			};

		public:
			ComponentSystemContainer(int maxParticleCount);

			int startIdx() override;
			int endIdx() override;

			template <typename T>
			void registerComponent(std::vector<T> initData = {});

			template <typename T>
			T &getComponent(const int idx) const;

			template <typename T>
			std::shared_ptr<ComponentPool<T>> getComponent() const;

			inline ParticleContainerType getType() override { return ParticleContainerType::SoA_CS; }
			inline unsigned int size() override { return maxParticles; }

			std::shared_ptr<ParticleContainerIterator> begin() override;
			std::shared_ptr<ParticleContainerIterator> end() override;
			std::shared_ptr<ParticleContainerIterator> getUnusedParticlesIterator() override;
			std::shared_ptr<RangeParticleContainerIterator> createRangeIterator() override;

			template <typename T>
			std::shared_ptr<component_iterator<T>> begin();
			template <typename T>
			std::shared_ptr<component_iterator<T>> end();

			void setLiveParticlePredicate(PredicateFunction predicate);
			void setDeadParticlePredicate(PredicateFunction predicate);

		protected:
			int maxParticles;

			std::unordered_map<const char *, std::shared_ptr<ComponentPoolBase>> components;

			std::shared_ptr<unused_particles_iterator> unusedParticlesIterator;
			PredicateFunction liveParticlePredicate = [](const int, const ComponentSystemContainer &) { return true; };
			PredicateFunction deadParticlePredicate = [](const int, const ComponentSystemContainer &) { return true; };
		};
	}
}

inline int ge::particle::ComponentSystemContainer::startIdx()
{
	return 0;
}

inline int ge::particle::ComponentSystemContainer::endIdx()
{
	return maxParticles - 1;
}

template<typename T>
inline void ge::particle::ComponentSystemContainer::registerComponent(std::vector<T> initData)
{
	const char* typeName = typeid(T).name();

	assert(components.find(typeName) == components.end() && "Registering component type more than once.");

	components.insert({ typeName, std::make_shared<ComponentPool<T>>(maxParticles, initData) });
}

template<typename T>
inline T & ge::particle::ComponentSystemContainer::getComponent(const int idx) const
{
	assert(idx < maxParticles);

	const char* typeName = typeid(T).name();

	const auto component = components.find(typeName);

	assert(component != components.end() && "Component not registered before use.");

	auto &componentPool = std::static_pointer_cast<ComponentPool<T>>(component->second);

	return componentPool->get(idx);
}

template <typename T>
std::shared_ptr<ge::particle::ComponentPool<T>> ge::particle::ComponentSystemContainer::getComponent() const
{
	const char* typeName = typeid(T).name();

	const auto component = components.find(typeName);

	assert(component != components.end() && "Component not registered before use.");

	auto &componentPool = std::static_pointer_cast<ComponentPool<T>>(component->second);

	return componentPool;
}

template <typename T>
std::shared_ptr<ge::particle::ComponentSystemContainer::component_iterator<T>> ge::particle::ComponentSystemContainer::
begin()
{
	return std::make_shared<component_iterator<T>>(shared_from_this());
}

template <typename T>
std::shared_ptr<ge::particle::ComponentSystemContainer::component_iterator<T>> ge::particle::ComponentSystemContainer::
end()
{
	return std::make_shared<component_iterator<T>>(shared_from_this(), size());
}

inline void ge::particle::ComponentSystemContainer::setLiveParticlePredicate(PredicateFunction predicate)
{
	liveParticlePredicate = predicate;
}

inline void ge::particle::ComponentSystemContainer::setDeadParticlePredicate(PredicateFunction predicate)
{
	deadParticlePredicate = predicate;
	std::static_pointer_cast<filter_iterator>(getUnusedParticlesIterator())->setPredicate(predicate);
}
