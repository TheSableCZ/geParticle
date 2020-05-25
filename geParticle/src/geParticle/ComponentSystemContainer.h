#pragma once

#include <geParticle/ParticleContainer.h>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <geParticle/ComponentPool.h>
#include <geParticle/StandardParticleComponents.h>

namespace ge {
	namespace particle {

		class ComponentSystemContainer : public StructureOfArraysContainer {
		public:
			ComponentSystemContainer(int maxParticleCount, bool registerStandardComponents = false);

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
	const char* typeName = typeid(T).name();

	assert(components.find(typeName) == components.end() && "Registering component type more than once.");

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
