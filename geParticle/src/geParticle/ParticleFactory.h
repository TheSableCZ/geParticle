#pragma once

#include <memory>
#include <geParticle/Particle.h>
#include <geParticle/ParticleContainer.h>

namespace ge
{
    namespace particle
    {

		template <class T>
        class ParticleFactory
        {
        public:
            //virtual Particle getParticle() = 0;
			//virtual void initParticle(Particle &p) = 0;

			virtual T &createParticle(std::shared_ptr<ParticleContainer> particles) = 0;
        };

        /*Particle BasicParticleFactory::getParticle() {
            Particle p;
            if (useTemplate)
                p = ptemplate;

            place(p);
            shoot(p);
            return p;
        }

        void BasicParticleFactory::place(Particle &p)
        {
            p.pos = glm::vec3(.0f, .0f, .0f);
        }

        void BasicParticleFactory::shoot(Particle &p)
        {
            p.velocity = glm::vec3(.0f, .0f, .0f);
        }*/
    }
}
