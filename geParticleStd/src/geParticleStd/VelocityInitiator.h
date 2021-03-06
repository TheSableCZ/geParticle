﻿/** @file VelocityInitiator.h
 *  @brief Init velocity attribute. Sphere shooter.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <glm/glm.hpp>
#include <geParticle/ParticleInitiator.h>
#include <geParticleStd/Export.h>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Init velocity attribute. Sphere shooter.
		 */
		class GEPARTICLESTD_EXPORT VelocityInitiator : public ParticleInitiator
		{
			const double PI = 3.14159265358979323846;
			const double PI_2 = 1.57079632679489661923;
			const double PI_4 = 0.78539816339744830962;

		public:
			VelocityInitiator() = default;
			VelocityInitiator(glm::vec2 speedRange, float spread = 1.5f, glm::vec3 maindir = glm::vec3(0.0f, 1.0f, 0.0f))
				: speedRange(speedRange), spread(spread), maindir(maindir)
			{}

			static float rangeRandom(glm::vec2 range);

			void init(std::shared_ptr<RangeParticleContainerIterator> range) override;

			//glm::vec2 thetaRange = glm::vec2(0, 0.5f * PI_4);
			//glm::vec2 phiRange = glm::vec2(0, 2 * PI);
			glm::vec2 speedRange = glm::vec2(6, 8);
			float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(0.0f, 1.0f, 0.0f);
		};
	}
}
