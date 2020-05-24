#include <geParticle/ComputeProgramWrapper.h>

ge::particle::ComputeProgramWrapper::ComputeProgramWrapper(std::string shaderSource)
{
	auto csShader = std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER, shaderSource);
	program = std::make_shared<ge::gl::Program>(csShader);

	if (program->getContext().glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not create shader-program.");
	}

	program->getComputeWorkGroupSize(workgroupSize);
}

void ge::particle::ComputeProgramWrapper::checkUniform(std::string name)
{
	auto programInfo = program->getInfo();

	if (programInfo->uniforms.find(name) == programInfo->uniforms.end()) {
		throw std::invalid_argument("ERROR: There is no such uniform: " + name);
	}
}

void ge::particle::ComputeProgramWrapper::dispatch(int count)
{
	// ceil
	unsigned int xGroups = (count + workgroupSize[0] - 1) / workgroupSize[0];

	program->dispatch(xGroups, workgroupSize[1], workgroupSize[2]);
	program->getContext().glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

std::shared_ptr<ge::gl::Buffer> ge::particle::ComputeProgramWrapper::createBuffer(GLsizeiptr size, GLuint bindIndex)
{
	auto buffer = std::make_shared<ge::gl::Buffer>(size);
	buffer->bind(GL_SHADER_STORAGE_BUFFER);

	buffer->bindBase(GL_SHADER_STORAGE_BUFFER, bindIndex);

	return buffer;
}
