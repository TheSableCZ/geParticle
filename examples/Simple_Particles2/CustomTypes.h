#pragma once

#include <geParticle/ComponentSystemContainer.h>
#include <geParticle/Particle.h>
//#include <geParticle/AoSParticleFactory.h>
//#include <geParticle/SoAParticleFactory.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ParticleInitiator.h>
#include <geParticle/ParticleAffector.h>
#include <geParticle/ArrayOfStructsContainer.h>

namespace ge {
	namespace particle {
		struct CustomParticle : public Particle {
			glm::vec3 color;
		};

		struct Color {
			glm::vec3 color;
		};

		class ColorInitiator : public ParticleInitiator
		{
		public:
			void init(std::shared_ptr<RangeParticleContainerIterator> range) override {
				if (range->getContainerType() == ParticleContainerType::SoA_CS) {
					auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
					auto end = range->end();

					for (; (*begin) != (*end); (*begin)++) {
						//begin->getComponent<Color>().color = glm::vec3(1.f, 0.f, 0.f);
						begin->getComponent<Color>().color = glm::vec3(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)));
					}
				}
			}
		};

		class VelocityInitiator : public ParticleInitiator
		{
			void init(std::shared_ptr<RangeParticleContainerIterator> range)
			{
				if (range->getContainerType() == ParticleContainerType::SoA_CS) {
					auto begin = std::static_pointer_cast<ComponentSystemContainer::range_iterator>(range->begin());
					auto end = range->end();

					for (; (*begin) != (*end); (*begin)++) {
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
					auto begin = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::range_iterator>(range->begin());
					auto end = range->end();

					for (; (*begin) != (*end); (*begin)++) {
						auto &particle = **begin;

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

						particle.velocity = maindir + randomdir * spread;
					}
				}
			}
		};

		class ColorAffector : public ParticleAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override {
				if (particles->getType() == ParticleContainerType::SoA_CS) {
					auto begin = std::static_pointer_cast<ComponentSystemContainer::iterator>(particles->begin());
					auto end = particles->end();

					for (; (*begin) != (*end); (*begin)++) {
						if (begin->getComponent<LifeData>().livingFlag == false) {
							begin->getComponent<Color>().color = glm::vec3(0.f, 0.f, 0.f);
						}
					}
				}
			}
		};

		/*class CustomFactory : public AoSParticleFactory {
			void initParticle(Particle &p) override {
				CustomParticle &cp = static_cast<CustomParticle &>(p);

				p.life = core::time_unit(.5f);
				//p.velocity = glm::vec3(0.5f, 0.5f, 0.f);
				cp.color = glm::vec3(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)));
				//cp.color = glm::vec3(0.f, 0.f, 1.f);

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

				p.velocity = maindir + randomdir * spread;
			}
		};

		class CSCustomFactory : public SoAParticleFactory {
			void initParticle(int idx, std::shared_ptr<ParticleContainer> particles) override {
				auto container = std::static_pointer_cast<ComponentSystemContainer>(particles);

				auto &v = container->getComponent<Velocity>(idx);
				auto &l = container->getComponent<LifeData>(idx);
				auto &c = container->getComponent<Color>(idx);

				l.life = core::time_unit(.5f);
				//v.velocity = glm::vec3(0.5f, 0.5f, 0.f);
				c.color = glm::vec3(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)));

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

				v.velocity = maindir + randomdir * spread;
			}
		};*/
	}
}
