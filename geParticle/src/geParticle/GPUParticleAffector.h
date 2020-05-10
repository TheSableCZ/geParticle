#pragma once

#include <geParticle/ParticleAffector.h>
#include <string>
#include <geGL/Program.h>
#include <iostream>

namespace ge {
	namespace particle {

		class GPUParticleAffector : public ParticleAffector {
		public:
			GPUParticleAffector(std::string shaderSource, int workgroupSize);
			void affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) override;

		protected:
			std::shared_ptr<ge::gl::Program> program;
			int workgroupSize;
		};

	}
}

inline ge::particle::GPUParticleAffector::GPUParticleAffector(std::string shaderSource, int workgroupSize)
	: workgroupSize(workgroupSize), ParticleAffector()
{
	auto csShader = std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER, shaderSource);
	std::cout << "Log: " << csShader->getInfoLog() << std::endl;

	program = std::make_shared<ge::gl::Program>(csShader);
	std::cout << "Program Log: " << program->getInfoLog() << std::endl;

	//program->set1f("dt", 1.f);
}

void ge::particle::GPUParticleAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	program->set1f("dt", dt.count());
	program->set1ui("particleCount", particles->size());

	program->dispatch((particles->size()/workgroupSize) + 1, 1, 1);
	program->getContext().glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
