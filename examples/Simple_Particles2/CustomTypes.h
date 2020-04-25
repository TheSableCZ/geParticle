#pragma once

#include <geParticle/AoSParticleFactory.h>

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
	}
}
