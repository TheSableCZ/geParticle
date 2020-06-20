#pragma once

#include <geParticle/ParticleContainer.h>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <geParticle/ComponentPool.h>
#include <geParticle/StandardParticleComponents.h>
#include <geParticle/ParticleContainerIterator.h>

namespace ge {
	namespace particle {

		class ComponentSystemContainer : public StructureOfArraysContainer, public std::enable_shared_from_this<ComponentSystemContainer> {

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

				void setPredicate(std::function<bool(const int, const ComponentSystemContainer &)> predicate) { this->predicate = predicate; }

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

				std::function<bool(const int, const ComponentSystemContainer &)> predicate = [](const int, const ComponentSystemContainer &) { return true; };
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
			ComponentSystemContainer(int maxParticleCount, bool registerStandardComponents = false);

			int startIdx() override;
			int endIdx() override;

			template <typename T>
			void registerComponent(std::vector<T> initData = {});

			int createParticle() override;
			template <typename T>
			T &getComponent(int idx);

			inline ParticleContainerType getType() override { return ParticleContainerType::SoA_CS; }
			inline unsigned int size() override { return maxParticles; }

			std::shared_ptr<ParticleContainerIterator> begin() override;
			std::shared_ptr<ParticleContainerIterator> end() override;
			std::shared_ptr<ParticleContainerIterator> getUnusedParticlesIterator() override;
			std::shared_ptr<RangeParticleContainerIterator> createRangeIterator() override;

		protected:
			int maxParticles;

			std::unordered_map<const char *, std::shared_ptr<IComponentPool>> components;

			int findUnusedParticle();
			int lastUsedParticle = 0;

			std::shared_ptr<filter_iterator> unusedParticlesIterator;
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
inline T & ge::particle::ComponentSystemContainer::getComponent(int idx)
{
	assert(idx < maxParticles);

	const char* typeName = typeid(T).name();

	auto component = components.find(typeName);

	assert(component != components.end() && "Component not registered before use.");

	auto componentPool = std::static_pointer_cast<ComponentPool<T>>(component->second);

	return componentPool->get(idx);
}
