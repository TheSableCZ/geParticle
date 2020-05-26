
#include <geParticle/BasicParticleEmitter.h>
#include <geParticle/ParticleContainer.h>
#include <geParticle/AoSParticleFactory.h>
#include <geParticle/SoAParticleFactory.h>

/*void ge::particle::BasicParticleEmitter::emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles) {
    int newParticlesCount = getNumOfParticlesToCreate(dt);

	if (particles->getType() == ParticleContainer::AoS) {
		auto container = std::static_pointer_cast<ArrayOfStructsContainer>(particles);

		for (int i = 0; i < newParticlesCount; i++) {
			//Particle p = particleFactory->getParticle();
			//p.life = core::time_unit(1.0); // This particle will live 5 seconds.

			//particleSystem->addParticle(p);

			Particle &p = container->createParticle();
			particleFactory->initParticle(p);
		}
	}
}*/

ge::particle::BasicParticleEmitter::BasicParticleEmitter(int particlesPerSecond)
	: particlesPerSecond(particlesPerSecond)
{
}

void ge::particle::BasicParticleEmitter::emitParticles(core::time_unit dt, std::shared_ptr<ParticleContainer> particles)
{
	int newParticlesCount = getNumOfParticlesToCreate(dt);

	if (particles->getType() == ParticleContainer::AoS) {
		auto container = std::static_pointer_cast<ArrayOfStructsContainer>(particles);

		for (int i = 0; i < newParticlesCount; i++) {
			Particle &p = factory ? std::static_pointer_cast<AoSParticleFactory>(factory)->createParticle(particles) : container->createParticle();

			p.livingFlag = true;

			initAoSParticle(p);
		}
	}

	if (particles->getType() == ParticleContainer::SoA_CS) {
		auto container = std::static_pointer_cast<ComponentSystemContainer>(particles);

		for (int i = 0; i < newParticlesCount; i++) {
			int idx = factory ? std::static_pointer_cast<SoAParticleFactory>(factory)->createParticle(particles) : container->createParticle();

			auto &l = container->getComponent<LifeData>(idx);

			l.livingFlag = true;

			initComponentSystemParticle(idx, container);
		}
	}
}

unsigned int ge::particle::BasicParticleEmitter::getNumOfParticlesToCreate(core::time_unit dt) {
    double realValue = (dt.count() * (float)particlesPerSecond);

    int newParticles = (int)realValue;
    carryOver += realValue - (double)newParticles;
    if (carryOver > 1.0) {
        newParticles++;
        carryOver -= 1.0;
    }

    /*if (newParticles > (int)(0.016f * (float)particlesPerSecond))
        newParticles = (int)(0.016f * (float)particlesPerSecond);*/

    return newParticles;
}
