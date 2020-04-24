#include <geParticle/ParticleSystem.h>

#include <chrono>
#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleAffector.h>
#include <geParticle/ParticleContainer.h>
#include <geParticle/ParticleRenderer.h>

//ge::particle::ParticleSystem::ParticleSystem(int MaxParticles) : MaxParticles{MaxParticles}
//{
//    //lastTime = core::time_point::clock::now();
//    //particleContainer = new Particle[MaxParticles];
//    //particleEmitter = std::make_unique<BasicParticleEmitter>(this);
//}

ge::particle::ParticleSystem::ParticleSystem(std::shared_ptr<ParticleContainer> container)
{
	particleContainer = container;
}

/*ge::particle::ParticleSystem::~ParticleSystem()
{
	delete[] particleContainer;
}*/

/*int ge::particle::ParticleSystem::findUnusedParticle(){
    for(int i=LastUsedParticle; i<MaxParticles; i++){
        if (!particleContainer[i].livingFlag){
            LastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<LastUsedParticle; i++){
        if (!particleContainer[i].livingFlag){
            LastUsedParticle = i;
            return i;
        }
    }

    //return 0; // All particles are taken, override the first one

	// All particles are taken, override random particle
	return rand() % MaxParticles;
}*/

/*void ge::particle::ParticleSystem::addParticle(ge::particle::Particle p)
{
    particleContainer[findUnusedParticle()] = p;
}*/

/*ge::particle::Particle & ge::particle::ParticleSystem::createParticle()
{
	return particleContainer[findUnusedParticle()];
}*/

void ge::particle::ParticleSystem::update(const ge::core::time_point &t) {
    core::time_unit dt = t-lastTime;
	lastTime = t;

	// TODO: std for_each ??

    /*for (int i = 0; i < MaxParticles; i++) {
        Particle &p = particleContainer[i];

        if (p.livingFlag) {
            for (auto &particleAffector : particleAffectors)
                particleAffector->affect(dt, p);
        }
    }*/

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
