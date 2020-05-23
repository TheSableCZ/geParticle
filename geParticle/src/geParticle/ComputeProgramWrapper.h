#pragma once

#include <string>
#include <geGL/Program.h>
#include <geGL/Buffer.h>

namespace ge {
	namespace particle {

		class ComputeProgramWrapper {
		public:
			ComputeProgramWrapper(std::string shaderSource);
			
			void checkUniform(std::string name);
			/*template <typename... Args>
			void setUniformValue(std::string name, Args... args);*/

			void dispatch(int count);

			std::shared_ptr<ge::gl::Buffer> createBuffer(GLsizeiptr size, GLuint bindIndex);

		protected:
			std::shared_ptr<ge::gl::Program> program;
			int workgroupSize[3];
		};

	}
}

/*template<typename ...Args>
inline void ge::particle::ComputeProgramWrapper::setUniformValue(std::string name, Args ...args)
{
	program->set(name, args...);
}*/
