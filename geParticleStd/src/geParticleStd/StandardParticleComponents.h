/** @file StandardParticleComponents.h
 *  @brief Standard particle attributes used by ComponentSystemContainer.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <glm/glm.hpp>
#include <geCore/Updatable.h>

namespace ge {
	namespace particle {
		struct LifeData {
			core::time_unit life;
			core::time_unit totalLifeTime;
			bool livingFlag = false;
		};

		/*
		For CPU is better to have position and velocity divided because data transfers.
		*/
		struct Position {
			glm::vec3 position;
		};

		struct Velocity {
			glm::vec3 velocity;
		};

		struct Color {
			glm::vec4 color;
		};

		/*
		Simple data type for GPU buffer.
		*/
		struct GPULifeData {
			float life;
			bool livingFlag = false;
		};

		struct GPUMassPointData {
			// vec4 because struct alignment to 16 bytes in glsl shaders
			glm::vec4 position;
			glm::vec4 velocity;
		};
	}
}