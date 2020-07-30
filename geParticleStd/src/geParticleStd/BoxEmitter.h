/** @file BoxEmitter.h
 *  @brief Emitter of box shape.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <random>
#include <glm/glm.hpp>
#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleInitiator.h>
#include <geParticleStd/ConstantRateCounter.h>
#include <geParticleStd/StandardEmitter.h>
#include <glm/gtx/transform.hpp>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Emitter of box shape.
		 */
		class GEPARTICLESTD_EXPORT BoxEmitter : public StandardEmitter
		{
			class BoxInitiator : public ParticleInitiator
			{
			public:
				BoxInitiator(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
					: x(glm::vec2(xMin, xMax)), y(glm::vec2(yMin, yMax)), z(glm::vec2(zMin, zMax)),
					generator(core::time_point::clock::now().time_since_epoch().count()),
					rotMat(glm::rotate(0.f, glm::vec3(0.f, 1.f, 0.f)))
				{}

				void init(std::shared_ptr<RangeParticleContainerIterator> range) override;

			protected:
				glm::vec3 getRandPos();
				
				glm::vec2 x;
				glm::vec2 y;
				glm::vec2 z;
				std::mt19937 generator;

				glm::mat4 rotMat;
			};

		public:
			BoxEmitter(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, core::time_unit& life, const std::shared_ptr<Counter>& counter);
		};
	}
}
