
#include <geParticle/BasicParticleEmitter.h>
#include <geParticle/ParticleContainer.h>

/*ge::particle::BasicParticleEmitter::BasicParticleEmitter(std::shared_ptr<ParticleSystem> ps) : particleSystem{ps}
{
    particleFactory = std::make_unique<BasicParticleFactory>(); // v konstruktoru, build pattern??
}*/

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

int ge::particle::BasicParticleEmitter::getNumOfParticlesToCreate(core::time_unit dt) {
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
