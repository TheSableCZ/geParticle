#include "TrailEmitter.h"


#include "FireworkExample.h"
#include "geParticleStd/LifeTimeInitiator.h"

ge::examples::TrailEmitter::TrailEmitter()
{
	initiators.emplace_back(std::make_shared<particle::LifeTimeInitiator>(core::time_unit(1.f)));
}

void ge::examples::TrailEmitter::emitParticles(core::time_unit dt,
	std::shared_ptr<particle::ParticleContainer> particles)
{
	auto unusedParticlesIterator = std::static_pointer_cast<particle::ComponentSystemContainer::iterator>(particles->getUnusedParticlesIterator());
	auto newRangeIterator = particles->createRangeIterator();

	while (!posQueue.empty())
	{
		auto point = posQueue.back();
		posQueue.pop_back();
		auto color = colorQueue.back();
		colorQueue.pop_back();

		//for (int i = 0; i < 10; i++) {
		auto newIdx = unusedParticlesIterator->getIndex();
		newRangeIterator->addIndex(newIdx);

		unusedParticlesIterator->getComponent<Type>().type = 3;
		unusedParticlesIterator->getComponent<particle::Position>().position = point;
		unusedParticlesIterator->getComponent<particle::Velocity>().velocity = glm::vec3(0, 0, 0);

		color.a -= 0.3;
		unusedParticlesIterator->getComponent<particle::Color>().color = color;
		unusedParticlesIterator->getComponent<OrigColor>().color = color;
		//unusedParticlesIterator->getComponent<particle::Color>().color = glm::vec4(.66f, .66f, .66f, .7f);

		unusedParticlesIterator->getComponent<Size>().size = glm::vec2(0.08f, 0.08f);

		(*unusedParticlesIterator)++;
		//}
	}

	for (auto &initiator : initiators)
		initiator->init(newRangeIterator);
}

void ge::examples::TrailEmitter::addParticle(glm::vec3 pos, glm::vec4 color)
{
	posQueue.emplace_back(pos);
	colorQueue.emplace_back(color);
}
