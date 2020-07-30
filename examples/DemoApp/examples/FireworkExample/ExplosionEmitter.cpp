#include "ExplosionEmitter.h"


#include "FireworkExample.h"
#include "geParticle/ComponentSystemContainer.h"
#include "geParticleStd/LifeTimeInitiator.h"
#include "geParticleStd/StandardParticleComponents.h"
#include "geParticleStd/VelocityInitiator.h"

ge::examples::ExplosionEmitter::ExplosionEmitter()
{
    auto time = core::time_unit(1.f);
	initiators.emplace_back(std::make_shared<particle::LifeTimeInitiator>(time));
	initiators.emplace_back(std::make_shared<particle::VelocityInitiator>(glm::vec2(10, 10), 5, glm::vec3(0, 0, 0)));
}

constexpr int explosionNum = 20;

void ge::examples::ExplosionEmitter::emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles)
{
	if (!posQueue.empty()) {
		auto unusedParticlesIterator = particles->getUnusedParticlesIterator();
		auto newRangeIterator = particles->createRangeIterator();

		newRangeIterator->addIndexesFrom(unusedParticlesIterator, explosionNum * posQueue.size());
		auto it = std::static_pointer_cast<particle::ComponentSystemContainer::range_iterator>(newRangeIterator->begin());
		
		while (!posQueue.empty())
		{
			
			auto point = posQueue.back();
			posQueue.pop_back();
			auto color = colorQueue.back();
			colorQueue.pop_back();

			for (int i = 0; i < explosionNum; i++) {
				//auto newIdx = unusedParticlesIterator->getIndex();
				//newRangeIterator->addIndex(newIdx);

				it->getComponent<Type>().type = 2;
				it->getComponent<particle::Position>().position = point;
				it->getComponent<particle::Color>().color = color;
				it->getComponent<Size>().size = glm::vec2(0.1f, 0.1f);

				++(*it);

				//(*unusedParticlesIterator)++;
			}
		}

		for (auto &initiator : initiators)
			initiator->init(newRangeIterator);
	}
}

void ge::examples::ExplosionEmitter::addExplosion(glm::vec3 pos, glm::vec4 color)
{
	posQueue.emplace_back(pos);
	colorQueue.emplace_back(color);
}
