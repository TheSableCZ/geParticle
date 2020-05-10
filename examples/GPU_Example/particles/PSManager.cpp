#include "PSManager.h"
#include <geParticle/ParticleSystem.h>
#include <geParticle/GPUParticleContainer.h>
#include <geParticle/GPUParticleAffector.h>
#include <geParticle/GPUParticleEmitter.h>
#include "SimpleParticleRenderer.h"
#include <geUtil/Text.h>
#include <geParticle/ComponentSystemContainer.h>
#include <random>

#define MAX_PARTICLES 100

void ge::examples::PSManager::initialize()
{
	pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES);

	pc->registerComponent<ge::particle::MassPointData>();
	pc->registerComponent<ge::particle::GPULifeData>();

	distributeParticles(pc);

	pc->initBuffers();
	pc->bindComponentBase<ge::particle::MassPointData>(0);
	pc->bindComponentBase<ge::particle::GPULifeData>(1);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	auto affector = std::make_shared<ge::particle::GPUParticleAffector>(ge::util::loadTextFile(APP_RESOURCES"/shaders/affector.glsl"));
	ps->addAffector(affector);

	auto emitter = std::make_shared<ge::particle::GPUParticleEmitter>(ge::util::loadTextFile(APP_RESOURCES"/shaders/emitter.glsl"), 1);
	ps->addEmitter(emitter);

	/*auto lifeTimeAffector = std::make_shared<ge::particle::LifeTimeAffector>();
	ps->addAffector(lifeTimeAffector);

	auto linearMovementAffector = std::make_shared<ge::particle::LinearMovementAffector>();
	ps->addAffector(linearMovementAffector);*/

	auto particleRenderer = std::make_shared<ge::particle::SimpleParticleRenderer>(pc);
	ps->particleRenderer = particleRenderer;

	ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::examples::PSManager::update()
{
	//printParticles();

	ps->update(ge::core::time_point::clock::now());

	//printParticles();
}

void ge::examples::PSManager::distributeParticles(std::shared_ptr<ge::particle::GPUParticleContainer> container)
{
	float rndX, rndY, rndZ;
	auto fInitRadius = static_cast<float>(15);
	std::mt19937 eng;
	std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

	for (auto i = 0u; i < container->size(); ++i) {
		rndX = dist(eng);
		rndY = dist(eng);
		rndZ = dist(eng);

		container->getComponent<ge::particle::MassPointData>(i).position = glm::vec4(rndX, rndY, rndZ, 1.f);
	}
}

void ge::examples::PSManager::printParticles()
{
	auto vect = pc->getBufferData<ge::particle::MassPointData>();
	for (auto p : vect) {
		std::cout << "pos: " << p.position.x << ", " << p.position.y << ", " << p.position.z << ", " << p.position.w << std::endl;
		std::cout << "vel: " << p.velocity.x << ", " << p.velocity.y << ", " << p.velocity.z << std::endl << std::endl;
	}
	std::cout << "--------------------------------------------------" << std::endl;
}
