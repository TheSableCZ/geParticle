/** @file ComputeProgramWrapper.h
 *  @brief Wrapper for OpenGL (geGL) compute shader.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <string>
#include <geGL/Program.h>
#include <geGL/Buffer.h>
#include <geParticleGL/Export.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Wrapper for OpenGL (geGL) compute shader.
		 */
		class GEPARTICLEGL_EXPORT ComputeProgramWrapper {
		public:
			ComputeProgramWrapper(std::string shaderSource);

			/**
			 * @brief Check if uniform exists in shader source code. Throw exception if uniform does not exist.
			 * @param name Name of uniform.
			 */
			void checkUniform(std::string name);

			/**
			 * @brief Run dispatch on GPU.
			 * @param count Number of total calculations. It is recalculated to workgoups (workgroup size is defined in GLSL).
			 */
			void dispatch(int count);

			/**
			 * @brief Creates an SSBO and bind at index.
			 */
			std::shared_ptr<ge::gl::Buffer> createBuffer(GLsizeiptr size, GLuint bindIndex);

		protected:
			std::shared_ptr<ge::gl::Program> program;
			int workgroupSize[3];
		};

	}
}
