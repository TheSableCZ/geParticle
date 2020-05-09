#pragma once

#include <geParticle/ParticleContainer.h>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace ge {
	namespace particle {
		struct IComponent {};
		class IComponentPool {
		public:
			virtual void clear() = 0;
			virtual void *data() = 0;
		};

		struct LifeData : public IComponent {
			core::time_unit life;
			bool livingFlag = false;
		};

		struct MassPointData : public IComponent {
			glm::vec3 position, velocity;
		};

		class ComponentSystemContainer : public StructureOfArraysContainer {
		public:
			ComponentSystemContainer(int maxParticleCount, bool registerLifeData = true);

			int startIdx() override;
			int endIdx() override;

			template <typename T>
			void registerComponent();

			int createParticle() override;
			template <typename T>
			T &getComponent(int idx);

			inline ContainerType getType() override { return SoA_CS; }
			inline unsigned int size() override { return maxParticles; }

		protected:
			int maxParticles;

			std::unordered_map<const char *, std::shared_ptr<IComponentPool>> components;

			int findUnusedParticle();
			int lastUsedParticle = 0;
		};

		template <typename T>
		class ComponentPool : public IComponentPool {
		public:
			ComponentPool(int size) {
				pool = std::vector<T>(size);
			}

			T &get(int idx) {
				return pool[idx];
			}

			void clear() override {
				pool.clear();
			}

			void *data() override {
				return pool.data();
			}

		private:
			std::vector<T> pool;
		};
	}
}

inline ge::particle::ComponentSystemContainer::ComponentSystemContainer(int maxParticleCount, bool registerLifeData)
	: maxParticles(maxParticleCount), StructureOfArraysContainer()
{
	if (registerLifeData) {
		registerComponent<LifeData>();
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
inline void ge::particle::ComponentSystemContainer::registerComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value && "Type is not derived from IComponent.");

	const char* typeName = typeid(T).name();

	assert(components.find(typeName) == components.end() && "Registering component type more than once.");

	//auto pool = new ComponentPool<T>(maxParticles);
	//std::shared_ptr<IComponentPool> pool = std::make_shared<ComponentPool<T>>(maxParticles);

	components.insert({ typeName, std::make_shared<ComponentPool<T>>(maxParticles) });
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

inline int ge::particle::ComponentSystemContainer::findUnusedParticle()
{
	for (int i = lastUsedParticle; i < maxParticles; i++) {
		if (!getComponent<LifeData>(i).livingFlag) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (!getComponent<LifeData>(i).livingFlag) {
			lastUsedParticle = i;
			return i;
		}
	}

	// All particles are taken, override random particle
	return rand() % maxParticles;
}

inline int ge::particle::ComponentSystemContainer::createParticle()
{
	return findUnusedParticle();
}
