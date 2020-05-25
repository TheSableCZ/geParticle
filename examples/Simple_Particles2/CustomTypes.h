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

				p.velocity = glm::vec3(0.5f, 0.5f, 0.f);
				cp.color = glm::vec3(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)));
				//cp.color = glm::vec3(0.f, 0.f, 1.f);
			}
		};

		class CSCustomFactory : public SoAParticleFactory {
			void initParticle(int idx, std::shared_ptr<ParticleContainer> particles) override {
				auto container = std::static_pointer_cast<ComponentSystemContainer>(particles);

				auto &v = container->getComponent<Velocity>(idx);
				v.velocity = glm::vec3(0.5f, 0.5f, 0.f);
			}
		};
	}
}
