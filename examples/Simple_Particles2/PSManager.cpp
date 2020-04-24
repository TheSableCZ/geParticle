#include "PSManager.h"
#include <geParticle/ParticleSystem.h>
#include <geParticle/SimpleArrayOfStructsContainer.h>
#include <geParticle/ParticleAffector.h>
#include <geParticle/ParticleEmitter.h>
#include "SimpleParticleRenderer.h"

#define MAX_PARTICLES 100

void ge::examples::PSManager::initialize(std::shared_ptr<ge::gl::Context> glContext)
{
	auto pc = std::make_shared<ge::particle::SimpleArrayOfStructsContainer>(MAX_PARTICLES);
	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	auto lifeTimeAffector = std::make_shared<ge::particle::LifeTimeAffector>();
	ps->addAffector(lifeTimeAffector);

	auto linearMovementAffector = std::make_shared<ge::particle::LinearMovementAffector>();
	ps->addAffector(linearMovementAffector);

	auto pointEmitter = std::make_shared<ge::particle::PointEmitter>(10, glm::vec3(0, 0, 0));
	ps->addEmitter(pointEmitter);

	auto particleRenderer = std::make_shared<ge::particle::SimpleParticleRenderer>(glContext, MAX_PARTICLES);
	ps->particleRenderer = particleRenderer;

	ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::examples::PSManager::update()
{
	ps->update(ge::core::time_point::clock::now());
}
