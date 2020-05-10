#pragma once

#include <geParticle/ParticleAffector.h>
#include <string>
#include <geGL/Program.h>
#include <iostream>
#include <geParticle/ComputeProgramWrapper.h>

namespace ge {
	namespace particle {

		class GPUParticleAffector : public ParticleAffector, ComputeProgramWrapper {
		public:
			GPUParticleAffector(std::string shaderSource);
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;
		};

	}
}

inline ge::particle::GPUParticleAffector::GPUParticleAffector(std::string shaderSource)
	: ComputeProgramWrapper(shaderSource), ParticleAffector()
{
	/*auto csShader = std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER, shaderSource);
	std::cout << "Log: " << csShader->getInfoLog() << std::endl;

	program = std::make_shared<ge::gl::Program>(csShader);
	std::cout << "Program Log: " << program->getInfoLog() << std::endl;

	if (program->getContext().glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not create shader-program.");
	}

	auto programInfo = program->getInfo();

	assert(programInfo->uniforms.find("dt") != programInfo->uniforms.end() && "Uniform 'dt' not found in program.");
	*/
	/*try {
		program->set1f("dt", 0.f);
	}
	catch (const std::invalid_argument& ex) {
		std::cerr << "Bad affector program." << std::endl;
		//throw std::runtime_error("ERROR: Bad affector program.");
	}*/

	checkUniform("dt");
	checkUniform("particleCount");
}

void ge::particle::GPUParticleAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	program->set("dt", (float)dt.count());
	program->set("particleCount", particles->size());

	dispatch(particles->size());
	/*program->set1f("dt", dt.count());
	program->set1ui("particleCount", particles->size());

	program->dispatch((particles->size()/workgroupSize) + 1, 1, 1);
	program->getContext().glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);*/
}
