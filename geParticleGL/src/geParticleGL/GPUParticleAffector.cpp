#include <geParticleGL/GPUParticleAffector.h>

ge::particle::GPUParticleAffector::GPUParticleAffector(std::string shaderSource)
	: ParticleAffector(), ComputeProgramWrapper(shaderSource)
{
	checkUniform("dt");
	checkUniform("particleCount");
}

void ge::particle::GPUParticleAffector::affect(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	program->set("dt", (float)dt.count());
	program->set("particleCount", particles->size());

	dispatch(particles->size());
}
