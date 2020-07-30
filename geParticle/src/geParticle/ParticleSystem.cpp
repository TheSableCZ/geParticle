/** @file ParticleSystem.h
 *  @brief Particle system class.
 *  @author Jan Sobol xsobol04
 */

#include <geParticle/ParticleSystem.h>

#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleAffector.h>
#include <geParticle/ParticleContainer.h>

ge::particle::ParticleSystem::ParticleSystem(std::shared_ptr<ParticleContainer> container)
{
	particleContainer = container;
}

void ge::particle::ParticleSystem::update(const ge::core::time_point &t) {
    core::time_unit dt = t-lastTime;
	lastTime = t;

	for (auto &pa : particleAffectors)
		pa->affect(dt, particleContainer);

    for (auto &pe : particleEmitters)
		pe->emitParticles(dt, particleContainer);
}

void ge::particle::ParticleSystem::addEmitter(std::shared_ptr<ParticleEmitter> emitter)
{
	particleEmitters.push_back(emitter);
}

void ge::particle::ParticleSystem::addAffector(std::shared_ptr<ParticleAffector> affector)
{
	particleAffectors.push_back(affector);
}

void ge::particle::ParticleSystem::removeEmitter(std::shared_ptr<ParticleEmitter> emitter)
{
	auto found = std::find(particleEmitters.begin(), particleEmitters.end(), emitter);
	assert(found != particleEmitters.end() && "Emitter not found.");

	particleEmitters.erase(found);
}

void ge::particle::ParticleSystem::removeAffector(std::shared_ptr<ParticleAffector> affector)
{
	auto found = std::find(particleAffectors.begin(), particleAffectors.end(), affector);
	assert(found != particleAffectors.end() && "Affector not found.");

	particleAffectors.erase(found);
}

std::shared_ptr<ge::particle::ParticleContainer> ge::particle::ParticleSystem::getParticleContainer()
{
	return particleContainer;
}
