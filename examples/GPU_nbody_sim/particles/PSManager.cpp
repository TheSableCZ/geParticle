#include "PSManager.h"
#include <geParticle/ParticleSystem.h>
#include <geParticleGL/GPUParticleContainer.h>
#include <geParticleGL/GPUParticleAffector.h>
#include <geParticleGL/GPUParticleEmitter.h>
#include "SimpleParticleRenderer.h"
#include <geUtil/Text.h>
#include <geParticle/ComponentSystemContainer.h>
#include "BallGPUEmitter.h"

#define MAX_PARTICLES 2048

void ge::examples::PSManager::initialize()
{
	pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::GPU_ONLY);

	pc->registerComponent<ge::particle::GPUMassPointData>(false, distributeParticles());
	//pc->registerComponent<ge::particle::GPULifeData>();

	pc->bindComponentBase<ge::particle::GPUMassPointData>(0);
	//pc->bindComponentBase<ge::particle::GPULifeData>(1);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	auto affector = std::make_shared<ge::particle::GPUParticleAffector>(ge::util::loadTextFile(APP_RESOURCES"/shaders/affector.glsl"));
	ps->addAffector(affector);

	//auto emitter = std::make_shared<ge::particle::BallGPUEmitter>(ge::util::loadTextFile(APP_RESOURCES"/shaders/emitter.glsl"), 500, MAX_PARTICLES, 15);
	//ps->addEmitter(emitter);

	particleRenderer = std::make_shared<ge::particle::SimpleParticleRenderer>(pc);
	//ps->particleRenderer = particleRenderer;

	ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::examples::PSManager::update()
{
	//printParticles();

	ps->update(ge::core::time_point::clock::now());
	particleRenderer->render(pc);

	//printParticles();
}

/*void ge::examples::PSManager::refreshRandomBuffer()
{
	auto fInitRadius = static_cast<float>(10);
	std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

	float *buff = (float *)randomBuffer->map();
	for (auto i = 0u; i < MAX_PARTICLES * 3; i++) {
		buff[i] = dist(eng);
		//std::cout << buff[i] << " ";
	}

	randomBuffer->unmap();
}*/

/*
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

		container->getComponent<ge::particle::GPUMassPointData>(i).position = glm::vec4(rndX, rndY, rndZ, 1.f);
	}
}
*/

std::vector<ge::particle::GPUMassPointData> ge::examples::PSManager::distributeParticles()
{
	std::vector<ge::particle::GPUMassPointData> vect;

	float rndX, rndY, rndZ;
	auto fInitRadius = static_cast<float>(30);
	std::mt19937 eng;
	std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

	for (auto i = 0u; i < MAX_PARTICLES; ++i) {
		rndX = dist(eng);
		rndY = dist(eng);
		rndZ = dist(eng);

		ge::particle::GPUMassPointData item;
		item.position = glm::vec4(rndX, rndY, 0.f /*rndZ*/, 1.f);
		item.velocity = glm::vec4(0.f);

		vect.push_back(item);
	}

	return vect;
}

void ge::examples::PSManager::printParticles()
{
	//auto vect = pc->getBufferData<ge::particle::GPUMassPointData>();
	std::vector<ge::particle::GPUMassPointData> vect;
	pc->getBufferData(vect);
	
	//auto vect2 = pc->getBufferData<ge::particle::GPULifeData>();
	/*std::vector<ge::particle::GPULifeData> vect2;
	pc->getBufferData(vect2);*/

	for (int i = 0; i < vect.size(); i++) {
		auto &p = vect[i];
		std::cout << "pos: " << p.position.x << ", " << p.position.y << ", " << p.position.z << ", " << p.position.w << std::endl;
		std::cout << "vel: " << p.velocity.x << ", " << p.velocity.y << ", " << p.velocity.z << ", " << p.velocity.w << std::endl;
	}

	/*for (auto p : vect) {
		std::cout << "pos: " << p.position.x << ", " << p.position.y << ", " << p.position.z << ", " << p.position.w << std::endl;
		//std::cout << "vel: " << p.velocity.x << ", " << p.velocity.y << ", " << p.velocity.z << std::endl << std::endl;
	}
	for (auto p : vect2) {
		std::cout << "livingFlag: " << p.livingFlag << std::endl;
		//std::cout << "life: " << p.life << std::endl;
		//std::cout << "vel: " << p.velocity.x << ", " << p.velocity.y << ", " << p.velocity.z << std::endl << std::endl;
	}*/

	std::cout << "--------------------------------------------------" << std::endl;
}
