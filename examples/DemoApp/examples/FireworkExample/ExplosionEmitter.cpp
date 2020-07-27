#include "ExplosionEmitter.h"


#include "FireworkExample.h"
#include "geParticle/ComponentSystemContainer.h"
#include "geParticleStd/LifeTimeInitiator.h"
#include "geParticleStd/StandardParticleComponents.h"
#include "geParticleStd/VelocityInitiator.h"

ge::examples::ExplosionEmitter::ExplosionEmitter()
{
	initiators.emplace_back(std::make_shared<particle::LifeTimeInitiator>(core::time_unit(1.f)));
	initiators.emplace_back(std::make_shared<particle::VelocityInitiator>(glm::vec2(10, 10), 5, glm::vec3(0, 0, 0)));
}

constexpr int explosionNum = 20;

void ge::examples::ExplosionEmitter::emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles)
{
	//int newParticlesCount = getNumOfParticlesToCreate(dt);
	auto unusedParticlesIterator = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->getUnusedParticlesIterator());
	auto newRangeIterator = particles->createRangeIterator();

	while (!posQueue.empty())
	{
		auto point = posQueue.back();
		posQueue.pop_back();
		auto color = colorQueue.back();
		colorQueue.pop_back();
		
		for (int i = 0; i < explosionNum; i++) {
			auto newIdx = unusedParticlesIterator->getIndex();
			newRangeIterator->addIndex(newIdx);

			unusedParticlesIterator->getComponent<Type>().type = 2;
			unusedParticlesIterator->getComponent<particle::Position>().position = point;
			unusedParticlesIterator->getComponent<particle::Color>().color = color;
			unusedParticlesIterator->getComponent<Size>().size = glm::vec2(0.1f, 0.1f);
			
			(*unusedParticlesIterator)++;
		}
	}

	for (auto &initiator : initiators)
		initiator->init(newRangeIterator);
}

void ge::examples::ExplosionEmitter::addExplosion(glm::vec3 pos, glm::vec4 color)
{
	posQueue.emplace_back(pos);
	colorQueue.emplace_back(color);
}
