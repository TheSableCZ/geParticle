#pragma once

#include <geParticle/Particle.h>
#include <geParticle/AoSParticleFactory.h>
#include <geParticle/SoAParticleFactory.h>

namespace ge {
	namespace particle {
		struct CustomParticle : public Particle {
			glm::vec3 color;
		};

		class CustomFactory : public AoSParticleFactory {
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

		struct Color {
			glm::vec3 color;
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
		};
	}
}
