#pragma once

#include <geParticle/Particle.h>
#include <geCore/Updatable.h>

namespace ge
{
    namespace particle
    {
        class ParticleFactory
        {
        public:
            //virtual Particle getParticle() = 0;
			virtual void initParticle(Particle &p) = 0;
        };

        class BasicParticleFactory : public ParticleFactory
        {
        public:
            /*Particle getParticle() override // createParticle, operator init
            {
                Particle p;
                if (useTemplate)
                    p = ptemplate;

                p.livingFlag = true;

                place(p);
                shoot(p);
                return p;
            }*/

			void initParticle(Particle &p) {
				if (useTemplate)
					p = ptemplate;

				p.livingFlag = true;

				place(p);
				shoot(p);
			}
            inline void setParticleTemplate(Particle ptemplate) { this->ptemplate = ptemplate; useTemplate = true; }

        protected:
            virtual void place(Particle &p)
            {
                p.pos = glm::vec3(.0f, .0f, .0f);
            }

            virtual void shoot(Particle &p)
            {
                p.velocity = glm::vec3(.0f, .0f, .0f);
            }

        private:
            Particle ptemplate;
            bool useTemplate = false;
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
