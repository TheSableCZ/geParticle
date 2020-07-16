#pragma once
#include "ExampleManager.h"
#include "geParticle/ParticleInitiator.h"
#include "geParticle/ParticleRenderer.h"
#include "geParticle/ParticleSystem.h"
#include "geParticle/ParticleAffector.h"
#include "geParticleGL/GPUParticleContainer.h"
#include "geParticleStd/ColorInitiator.h"
#include "geParticleStd/PointEmitter.h"
#include "geParticleStd/StandardParticleComponents.h"

namespace ge
{
	namespace examples
	{
		class VelocityInitiator;
		
		class SimpleExample : public Example
		{
		public:
			SimpleExample(std::shared_ptr<particle::ParticleSystemManager> &manager);
			void init() override;
			void render() override;
			void reset() override;
			void renderGui() override;
			std::string getName() const override { return "Simple Particle System"; }

		private:
			void initAffectors();
			void renderGuiAffectorDetails(std::string affectorName, std::shared_ptr<particle::ParticleAffector> ptr) const;
			void changeAffector(std::shared_ptr<particle::ParticleAffector> affector, bool active) const;
			
			std::shared_ptr<particle::ParticleSystemManager> manager;
			std::shared_ptr<particle::ParticleSystem> ps;
			std::shared_ptr<particle::GPUParticleContainer> pc;
			std::shared_ptr<particle::ParticleRenderer> renderer;

			std::shared_ptr<particle::PointEmitter> emitter;
			std::shared_ptr<VelocityInitiator> velInitiator;
			std::shared_ptr<particle::ColorInitiator> colInitiator;

			std::vector<std::tuple<std::string, std::shared_ptr<particle::ParticleAffector>, bool>> allAffectors;
		};

		class VelocityInitiator : public particle::ParticleInitiator
		{
			const double PI = 3.14159265358979323846;
			const double PI_2 = 1.57079632679489661923;
			const double PI_4 = 0.78539816339744830962;
			
		public:
			static float rangeRandom(glm::vec2 range)
			{
				auto minimum = range.x;
				auto maximum = range.y;
				return minimum + (maximum - minimum) * rand() / RAND_MAX;
			}
			
			void init(std::shared_ptr<particle::RangeParticleContainerIterator> range) override
			{
				if (range->getContainerType() == particle::ParticleContainerType::SoA_CS) {
					auto begin = std::static_pointer_cast<particle::ComponentSystemContainer::range_iterator>(range->begin());
					auto end = range->end();

					for (; (*begin) != (*end); (*begin)++) {
						/*
						//float spread = 1.5f;
						glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);
						// Very bad way to generate a random direction; 
						// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
						// combined with some user-controlled parameters (main direction, spread, etc)
						glm::vec3 randomdir = glm::vec3(
							(rand() % 2000 - 1000.0f) / 1000.0f,
							(rand() % 2000 - 1000.0f) / 1000.0f,
							(rand() % 2000 - 1000.0f) / 1000.0f
						);

						(*begin).getComponent<particle::Velocity>().velocity = maindir + randomdir * spread;
						*/

						/*float theta = rangeRandom(thetaRange); //.get_random();
						float phi = rangeRandom(phiRange);
						float speed = rangeRandom(speedRange);
						*/

						// TODO: standard distribute
						float phi = rangeRandom(glm::vec2(0, 2 * PI)); //.get_random();
						float theta = acosf( rangeRandom(glm::vec2(-1, 1)) );
						float speed = rangeRandom(speedRange);
						float r = cbrt(rangeRandom(glm::vec2(0, 1)));

						glm::vec3 randomdir = glm::vec3(
							r * sinf(theta) * cosf(phi),
							r * sinf(theta) * sinf(phi),
							r * cosf(theta)
						);

						(*begin).getComponent<particle::Velocity>().velocity = speed * maindir + (randomdir) * spread;
					}
				}
			}

			//glm::vec2 thetaRange = glm::vec2(0, 0.5f * PI_4);
			//glm::vec2 phiRange = glm::vec2(0, 2 * PI);
			glm::vec2 speedRange = glm::vec2(6, 8);
			float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(0.0f, 1.0f, 0.0f);
		};
	}
}
