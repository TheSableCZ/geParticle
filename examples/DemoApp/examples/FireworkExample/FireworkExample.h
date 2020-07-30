#pragma once
#include <array>

#include "ExampleManager.h"
#include "geParticle/ParticleInitiator.h"
#include "geParticle/ParticleRenderer.h"
#include "geParticle/ParticleSystem.h"
#include "geParticleGL/GPUParticleContainer.h"
#include "geParticleStd/BoxEmitter.h"
#include "geParticleStd/ColorAffector.h"
#include "geParticleStd/GravityAffector.h"
#include "geParticleStd/PointEmitter.h"
#include "geParticleStd/StandardParticleComponents.h"

namespace ge
{
	namespace examples
	{
		class FireworkExample : public Example
		{
		public:
			FireworkExample(std::shared_ptr<particle::ParticleSystemManager> &manager);
			void init() override;
			void render() override;
			void reset() override;
			void renderGui() override;
			std::string getName() const override { return "Firework Effect"; }
			unsigned getContainerSize() const override;
			void stopEmitting() override;

		private:
			void setFountains(int particlesPerSecond);
			
			std::shared_ptr<particle::ParticleSystemManager> manager;
			std::shared_ptr<particle::ParticleSystem> ps;
			std::shared_ptr<particle::GPUParticleContainer> pc;
			std::shared_ptr<particle::ParticleRenderer> renderer;

			std::shared_ptr<particle::BoxEmitter> plane;
			std::array<std::shared_ptr<particle::PointEmitter>, 5> points;
		};

		// custom particle attributes
		struct Type
		{
			// 1 for not exploded, 2 for explosion particles, 3 for trails
			int type = 0;
		};
		
		struct ExplodedFlag
		{
			bool exploded = true;
		};

		struct Size
		{
			glm::vec2 size;
		};

		struct OrigColor
		{
			glm::vec4 color;
		};

		class FireworkAttribInitiator : public particle::ParticleInitiator
		{
			void init(std::shared_ptr<particle::RangeParticleContainerIterator> range) override
			{
				auto begin = std::static_pointer_cast<particle::ComponentSystemContainer::range_iterator>(range->begin());
				auto end = range->end();

				for (; (*begin) != (*end); (*begin)++)
				{
					begin->getComponent<Type>().type = 1;
					begin->getComponent<ExplodedFlag>().exploded = false;
					begin->getComponent<Size>().size = glm::vec2(0.1f, 0.1f);
				}
			}
		};

		class FountainInitiator : public particle::ParticleInitiator
		{
			void init(std::shared_ptr<particle::RangeParticleContainerIterator> range) override
			{
				auto begin = std::static_pointer_cast<particle::ComponentSystemContainer::range_iterator>(range->begin());
				auto end = range->end();

				for (; (*begin) != (*end); (*begin)++)
				{
					begin->getComponent<Type>().type = 2;
					begin->getComponent<ExplodedFlag>().exploded = true;
					begin->getComponent<Size>().size = glm::vec2(0.1f, 0.1f);
				}
			}
		};

		class TypedGravityAffector : public particle::GravityAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override
			{
				auto pi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<particle::LifeData>();
				auto end = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->end<particle::LifeData>();

				auto typePi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<Type>();

				//auto velPi = std::static_pointer_cast<ComponentSystemContainer>(particles)->begin<Velocity>();
				//auto pi = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->begin());
				//auto end = particles->end();

				for (pi; *pi != *(end); ++(*pi), ++(*typePi))
				{
					if (pi->get().livingFlag) {
						const auto type = typePi->get().type;

						float g = -9.81f;
						if (type == 2) g = -4.f;
						if (type == 3) g = 0.f;

						auto& v = pi->getComponent<particle::Velocity>();
						v.velocity += glm::vec3(0.0f, g, 0.0f) * (float)dt.count();
					}
				}
			}
		};

		class TypedColorAffector : public particle::ColorAffector
		{
		public:
			TypedColorAffector()
			{
				blendRatioPoints.emplace_back(std::make_pair(0.f, 0.f));
				blendRatioPoints.emplace_back(std::make_pair(.3f, 1.f));

				colorPoints.emplace_back(std::make_pair(30, glm::vec4(.66f, .66f, .66f, .7f)));
				colorPoints.emplace_back(std::make_pair(100, glm::vec4(.66f, .66f, .66f, .1f)));
			}
			
			void affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override
			{
				auto pi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<particle::LifeData>();
				auto end = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->end<particle::LifeData>();

				auto typePi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<Type>();
				
				//auto pi = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->begin());
				//auto end = particles->end();

				for (pi; *pi != *(end); ++(*pi), ++(*typePi))
				{
					if (typePi->get().type == 3)
					{
						auto& l = pi->get();
						auto& c = pi->getComponent<particle::Color>();

						if (l.livingFlag)
						{
							//begin->getComponent<Color>().color = glm::vec3(0.f, 0.f, 0.f);
							auto newCol = getColor(l.life, l.totalLifeTime);
							auto origColor = pi->getComponent<OrigColor>().color;

							auto percent = 1.f - l.life.count() / l.totalLifeTime.count();
							float blendRatio = 1.f;
							for (auto &point : blendRatioPoints)
							{
								if (point.first >= percent)
									break;

								blendRatio = point.second;
							}
							
							c.color = origColor + (newCol - origColor) * blendRatio;
						}
					}
				}
			}

		protected:
			std::vector<std::pair<float, float>> blendRatioPoints;
		};

		class SizeAffector : public particle::ParticleAffector
		{
			void affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override
			{
				auto pi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<particle::LifeData>();
				auto end = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->end<particle::LifeData>();

				auto typePi = std::static_pointer_cast<particle::ComponentSystemContainer>(particles)->begin<Type>();
				
				//auto pi = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->begin());
				//auto end = particles->end();

				for (pi; *pi != *(end); (*pi)++)
				{
					if (pi->get().livingFlag) {
						if (typePi->get().type == 3)
						{
							//auto& l = pi->getComponent<particle::LifeData>();
							auto& s = pi->getComponent<Size>();
							s.size += 0.07 * dt.count();
						}
					}
				}
			}
		};
	}
}
