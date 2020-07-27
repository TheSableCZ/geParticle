#pragma once
#include <random>
#include <glm/glm.hpp>
#include <geParticle/ParticleInitiator.h>
#include <geParticleStd/StandardEmitter.h>

namespace ge
{
	namespace particle
	{
		class BallEmitter : public StandardEmitter
		{
			class BallInitiator : public ParticleInitiator
			{
				const double PI = 3.14159265358979323846;
				
			public:
				BallInitiator(glm::vec3 center, float radius)
					: center(center), radius(radius),
					generator(core::time_point::clock::now().time_since_epoch().count())
				{}

				void init(std::shared_ptr<RangeParticleContainerIterator> range) override;

			protected:
				glm::vec3 getRandPos();
				
				glm::vec3 center;
				float radius;
				std::mt19937 generator;
			};

		public:
			BallEmitter(glm::vec3 center, float radius, core::time_unit& life, const std::shared_ptr<Counter>& counter);
		};
	}
}
