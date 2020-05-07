#pragma once

#include <geParticle/ParticleContainer.h>
#include <unordered_map>
#include <vector>

namespace ge {
	namespace particle {
		struct IComponent {};

		struct LifeData : public IComponent {
			core::time_unit life;
			bool livingFlag = false;
		};

		class ComponentSystemContainer : public StructureOfArraysContainer {
		public:
			ComponentSystemContainer(int maxParticleCount);

			int startIdx() override;
			int endIdx() override;

			template <typename T>
			void registerComponent();

			int createParticle() override;
			template <typename T>
			T &getComponent(int idx);

			inline ContainerType getType() override { return SoA_CS; }

		private:
			int maxParticles;

			std::unordered_map<const char *, std::vector<IComponent>> components;

			int findUnusedParticle();
			int lastUsedParticle = 0;
		};
	}
}

ge::particle::ComponentSystemContainer::ComponentSystemContainer(int maxParticleCount)
	: maxParticles(maxParticleCount), StructureOfArraysContainer()
{
	registerComponent<LifeData>();
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

	components.insert({ typeName, std::vector<T>(maxParticles) });
}

template<typename T>
inline T & ge::particle::ComponentSystemContainer::getComponent(int idx)
{
	assert(idx < maxParticles);

	const char* typeName = typeid(T).name();

	auto component = components.find(typeName);

	assert(component != components.end() && "Component not registered before use.");

	return (component->second)[idx];
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
