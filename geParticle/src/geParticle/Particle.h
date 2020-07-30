/** @file Particle.h
 *  @brief Base particle class for ArrayOfStructContainer.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <glm/glm.hpp>
#include <geCore/Updatable.h>

namespace ge
{
	namespace particle
	{
		/**
		 * @brief Base particle class for ArrayOfStructContainer.
		 */
		struct Particle
		{
			glm::vec3 pos, velocity;

			core::time_unit life;
			core::time_unit totalLifeTime;
			bool livingFlag = false;
		};
	}
}
