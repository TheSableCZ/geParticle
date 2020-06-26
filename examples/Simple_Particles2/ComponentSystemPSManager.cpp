#include "ComponentSystemPSManager.h"
#include <geParticle/ParticleSystem.h>
#include <geParticleGL/GPUParticleContainer.h>
#include <geParticleStd/StandardParticleAffectors.h>
#include <geParticleStd/PointEmitter.h>
#include "ComponentSystemRenderer.h"
#include "CustomTypes.h"
#include <geParticleStd/StandardParticleComponents.h>

#define MAX_PARTICLES 20

void ge::examples::ComponentSystemPSManager::initialize(std::shared_ptr<ge::gl::Context> glContext)
{
	pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::CPU_GPU);
	pc->registerComponent<ge::particle::LifeData>(false);
	pc->registerComponent<ge::particle::Position>(true);
	pc->registerComponent<ge::particle::Velocity>(false);

	pc->registerComponent<ge::particle::Color>(true);

	pc->setLiveParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool 
			{ return container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);
	pc->setDeadParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
			{ return !container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	auto lifeTimeAffector = std::make_shared<ge::particle::LifeTimeAffector>();
	ps->addAffector(lifeTimeAffector);

	auto linearMovementAffector = std::make_shared<ge::particle::LinearMovementAffector>();
	ps->addAffector(linearMovementAffector);

	ps->addAffector(std::make_shared<ge::particle::GravityAffector>());

	ps->addAffector(std::make_shared<ge::particle::ColorAffector>());

	auto pointEmitter = std::make_shared<ge::particle::PointEmitter>(10, glm::vec3(-1, -1, 0), core::time_unit(.5f));
	//auto customFactory = std::make_shared<ge::particle::CSCustomFactory>();

	//pointEmitter->setParticleFactory(customFactory);

	pointEmitter->initiators.push_back(std::make_shared<ge::particle::ColorInitiator>());
	pointEmitter->initiators.push_back(std::make_shared<ge::particle::VelocityInitiator>());
	ps->addEmitter(pointEmitter);

	renderer = std::make_shared<ge::particle::ComponentSystemRenderer>(glContext, MAX_PARTICLES, pc);
	//ps->particleRenderer = particleRenderer;

	//ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::examples::ComponentSystemPSManager::update()
{
	ps->update(ge::core::time_point::clock::now());
	renderer->render(pc);
}
