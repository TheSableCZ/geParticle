#pragma once

#include <string>
#include <geGL/Program.h>

namespace ge {
	namespace particle {

		class ComputeProgramWrapper {
		public:
			ComputeProgramWrapper(std::string shaderSource);
			
			void checkUniform(std::string name);
			/*template <typename... Args>
			void setUniformValue(std::string name, Args... args);*/

			void dispatch(int count);

		protected:
			std::shared_ptr<ge::gl::Program> program;
			int workgroupSize[3];
		};

	}
}

ge::particle::ComputeProgramWrapper::ComputeProgramWrapper(std::string shaderSource)
{
	auto csShader = std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER, shaderSource);
	program = std::make_shared<ge::gl::Program>(csShader);

	if (program->getContext().glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not create shader-program.");
	}

	program->getComputeWorkGroupSize(workgroupSize);
}

inline void ge::particle::ComputeProgramWrapper::checkUniform(std::string name)
{
	auto programInfo = program->getInfo();

	if (programInfo->uniforms.find(name) == programInfo->uniforms.end()) {
		throw std::invalid_argument("ERROR: There is no such uniform: " + name);
	}
}

/*template<typename ...Args>
inline void ge::particle::ComputeProgramWrapper::setUniformValue(std::string name, Args ...args)
{
	program->set(name, args...);
}*/

inline void ge::particle::ComputeProgramWrapper::dispatch(int count)
{
	program->dispatch((count / workgroupSize[0]) + 1, workgroupSize[1], workgroupSize[2]);
	program->getContext().glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
