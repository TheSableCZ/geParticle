#pragma once

#include <glm/glm.hpp>
#include <geCore/Updatable.h>

namespace ge {
	namespace particle {
		struct LifeData {
			core::time_unit life;
			bool livingFlag = false;
		};

		struct GPULifeData {
			float life;
			bool livingFlag = false;
		};

		struct MassPointData {
			// vec4 because struct alignment to 16 bytes in glsl shaders
			glm::vec4 position;
			glm::vec4 velocity;
		};
	}
}