/** @file TrailEmitter.cpp
 *  @brief Firework trail emitter.
 *  @author Jan Sobol xsobol04
 */

#include "TrailEmitter.h"


#include "FireworkExample.h"
#include "geParticleStd/LifeTimeInitiator.h"

ge::examples::TrailEmitter::TrailEmitter()
{
    auto time = core::time_unit(1.f);
	initiators.emplace_back(std::make_shared<particle::LifeTimeInitiator>(time));
}

void ge::examples::TrailEmitter::emitParticles(core::time_unit dt,
	std::shared_ptr<particle::ParticleContainer> particles)
{
	if (!posQueue.empty()) {
		auto unusedParticlesIterator = particles->getUnusedParticlesIterator();
		auto newRangeIterator = particles->createRangeIterator();
		newRangeIterator->addIndexesFrom(unusedParticlesIterator, posQueue.size());
		auto it = std::static_pointer_cast<particle::ComponentSystemContainer::range_iterator>(newRangeIterator->begin());
		

		while (!posQueue.empty())
		{
			auto point = posQueue.back();
			posQueue.pop_back();
			auto color = colorQueue.back();
			colorQueue.pop_back();

			//for (int i = 0; i < 10; i++) {
			//auto newIdx = unusedParticlesIterator->getIndex();
			//newRangeIterator->addIndex(newIdx);

			it->getComponent<Type>().type = 3;
			it->getComponent<particle::Position>().position = point;
			it->getComponent<particle::Velocity>().velocity = glm::vec3(0, 0, 0);

			color.a -= 0.3;
			it->getComponent<particle::Color>().color = color;
			it->getComponent<OrigColor>().color = color;
			//unusedParticlesIterator->getComponent<particle::Color>().color = glm::vec4(.66f, .66f, .66f, .7f);

			it->getComponent<Size>().size = glm::vec2(0.08f, 0.08f);

			++(*it);
			//}
		}

		for (auto &initiator : initiators)
			initiator->init(newRangeIterator);
	}
}

void ge::examples::TrailEmitter::addParticle(glm::vec3 pos, glm::vec4 color)
{
	posQueue.emplace_back(pos);
	colorQueue.emplace_back(color);
}
