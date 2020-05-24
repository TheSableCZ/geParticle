#include "PSManager.h"
#include <geParticle/ParticleSystem.h>
#include <geParticle/SimpleArrayOfStructsContainer.h>
#include <geParticle/StandardParticleAffectors.h>
#include <geParticle/PointEmitter.h>
#include "SimpleParticleRenderer.h"
#include "CustomTypes.h"

#define MAX_PARTICLES 100

void ge::examples::PSManager::initialize(std::shared_ptr<ge::gl::Context> glContext)
{
	auto pc = std::make_shared<ge::particle::SimpleArrayOfStructsContainer<ge::particle::CustomParticle>>(MAX_PARTICLES);
	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	auto lifeTimeAffector = std::make_shared<ge::particle::LifeTimeAffector>();
	ps->addAffector(lifeTimeAffector);

	auto linearMovementAffector = std::make_shared<ge::particle::LinearMovementAffector>();
	ps->addAffector(linearMovementAffector);

	auto pointEmitter = std::make_shared<ge::particle::PointEmitter>(1, glm::vec3(0, 0, 0));
	auto customFactory = std::make_shared<ge::particle::CustomFactory>();

	pointEmitter->setAoSParticleFactory(customFactory);

	ps->addEmitter(pointEmitter);

	auto particleRenderer = std::make_shared<ge::particle::SimpleParticleRenderer>(glContext, MAX_PARTICLES);
	ps->particleRenderer = particleRenderer;

	ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::examples::PSManager::update()
{
	ps->update(ge::core::time_point::clock::now());
}
