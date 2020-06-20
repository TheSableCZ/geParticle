#pragma once

//#include <geParticle/BasicParticleEmitter.h>
#include <glm/glm.hpp>
#include <memory>

#include <geParticle/ConstantRateEmitter.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticle/SimpleArrayOfStructsContainer.h>
#include <geParticle/StandardParticleComponents.h>

namespace ge {
	namespace particle {
		/*class PointEmitter : public BasicParticleEmitter
		{
		public:
			PointEmitter(int particlesPerSecond, glm::vec3 pos)
				: pos(pos), BasicParticleEmitter(particlesPerSecond) 
			{
			}

			void initAoSParticle(Particle &p) override {
				p.pos = pos;
			}

			void initComponentSystemParticle(int idx, std::shared_ptr<ComponentSystemContainer> container) override {
				auto &p = container->getComponent<Position>(idx);
				p.position = pos;
			}
			*/

			/*void emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override {
				int newParticlesCount = getNumOfParticlesToCreate(dt);

				if (particles->getType() == ParticleContainer::AoS) {
					auto container = std::static_pointer_cast<ArrayOfStructsContainer>(particles);

					for (int i = 0; i < newParticlesCount; i++) {
						Particle &p = factory ? std::static_pointer_cast<AoSParticleFactory>(factory)->createParticle(particles) : container->createParticle();

						p.pos = pos;
						p.livingFlag = true;
						// TODO: nekde v factory? minimalne parametrizovatelny
						p.life = core::time_unit(3.f);
					}
				}

				if (particles->getType() == ParticleContainer::SoA_CS) {
					auto container = std::static_pointer_cast<ComponentSystemContainer>(particles);

					for (int i = 0; i < newParticlesCount; i++) {
						int idx = factory ? std::static_pointer_cast<SoAParticleFactory>(factory)->createParticle(particles) : container->createParticle();

						auto &p = container->getComponent<Position>(idx);
						auto &l = container->getComponent<LifeData>(idx);

						p.position = pos;
						l.livingFlag = true;
						// TODO: nekde v factory?
						l.life = core::time_unit(3.f);
					}
				}
			}*/
		/*
		protected:
			glm::vec3 pos;
		};
		*/

		class PointEmitter : public ConstantRateEmitter
		{
			class PointInitiator : public ParticleInitiator
			{
			public:
				PointInitiator(glm::vec3 pos) : pos(pos) {}

				void init(std::shared_ptr<RangeParticleContainerIterator> range) 
				{
					if (range->getContainerType() == ParticleContainerType::SoA_CS) {
						auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
						auto end = range->end();

						for (; (*begin) != (*end); (*begin)++) {
							auto &l = begin->getComponent<LifeData>();
							l.livingFlag = true;
							l.life = core::time_unit(.5f);

							begin->getComponent<Position>().position = pos;

							float spread = 1.5f;
							glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);
							// Very bad way to generate a random direction; 
							// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
							// combined with some user-controlled parameters (main direction, spread, etc)
							glm::vec3 randomdir = glm::vec3(
								(rand() % 2000 - 1000.0f) / 1000.0f,
								(rand() % 2000 - 1000.0f) / 1000.0f,
								(rand() % 2000 - 1000.0f) / 1000.0f
							);

							(*begin).getComponent<Velocity>().velocity = maindir + randomdir * spread;
						}
					}
					else if (range->getContainerType() == ParticleContainerType::AoS) {
						auto begin = std::static_pointer_cast<SimpleArrayOfStructsContainer<Particle>::range_iterator>(range->begin());
						auto end = range->end();

						for (; (*begin) != (*end); (*begin)++) {
							auto &particle = (*begin);
							(*particle).livingFlag = true;
							(*particle).life = core::time_unit(.5f);

							(*particle).pos = pos;

							float spread = 1.5f;
							glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);
							// Very bad way to generate a random direction; 
							// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
							// combined with some user-controlled parameters (main direction, spread, etc)
							glm::vec3 randomdir = glm::vec3(
								(rand() % 2000 - 1000.0f) / 1000.0f,
								(rand() % 2000 - 1000.0f) / 1000.0f,
								(rand() % 2000 - 1000.0f) / 1000.0f
							);

							(*particle).velocity = maindir + randomdir * spread;
						}
					}
				}

			protected:
				glm::vec3 pos;
			};

		public:
			PointEmitter(int particlesPerSecond, glm::vec3 pos)
				: ConstantRateEmitter(particlesPerSecond)
			{
				initiators.push_back(std::make_shared<PointInitiator>(pos));
			}
		};
	}
}
