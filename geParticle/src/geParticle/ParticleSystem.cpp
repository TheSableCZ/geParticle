#include <geParticle/ParticleSystem.h>

#include <chrono>
#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleAffector.h>
#include <geParticle/ParticleContainer.h>
#include <geParticle/ParticleRenderer.h>

ge::particle::ParticleSystem::ParticleSystem(std::shared_ptr<ParticleContainer> container)
{
	particleContainer = container;
}

void ge::particle::ParticleSystem::update(const ge::core::time_point &t) {
    core::time_unit dt = t-lastTime;
	lastTime = t;

	// TODO: std for_each ??

	for (auto &pa : particleAffectors)
		pa->affect(dt, particleContainer);

    for (auto &pe : particleEmitters)
		pe->emitParticles(dt, particleContainer);

	if (particleRenderer)
		particleRenderer->render(particleContainer);
}

void ge::particle::ParticleSystem::addEmitter(std::shared_ptr<ParticleEmitter> emitter)
{
	particleEmitters.push_back(emitter);
}

void ge::particle::ParticleSystem::addAffector(std::shared_ptr<ParticleAffector> affector)
{
	particleAffectors.push_back(affector);
}
