#include "PSManager.h"
#include <geParticle/ParticleSystem.h>
#include <geParticle/SimpleArrayOfStructsContainer.h>
#include <geParticleStd/StandardParticleAffectors.h>
#include <geParticleStd/PointEmitter.h>
#include "SimpleParticleRenderer.h"
#include "CustomTypes.h"

#define MAX_PARTICLES 100

void ge::examples::PSManager::initialize(std::shared_ptr<ge::gl::Context> glContext)
{
	pc = std::make_shared<ge::particle::SimpleArrayOfStructsContainer<ge::particle::CustomParticle>>(MAX_PARTICLES);
	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	auto lifeTimeAffector = std::make_shared<ge::particle::LifeTimeAffector>();
	ps->addAffector(lifeTimeAffector);

	auto linearMovementAffector = std::make_shared<ge::particle::LinearMovementAffector>();
	ps->addAffector(linearMovementAffector);

	ps->addAffector(std::make_shared<ge::particle::GravityAffector>());

	auto pointEmitter = std::make_shared<ge::particle::PointEmitter>(10, glm::vec3(1, -1, 0));
	//auto customFactory = std::make_shared<ge::particle::CustomFactory>();

	//pointEmitter->setParticleFactory(customFactory);

	ps->addEmitter(pointEmitter);

	renderer = std::make_shared<ge::particle::SimpleParticleRenderer>(glContext, MAX_PARTICLES);
	//ps->particleRenderer = particleRenderer;

	ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::examples::PSManager::update()
{
	ps->update(ge::core::time_point::clock::now());
	renderer->render(pc);
}
