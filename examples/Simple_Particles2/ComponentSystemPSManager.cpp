#include "ComponentSystemPSManager.h"
#include <geParticle/ParticleSystem.h>
#include <geParticle/GPUParticleContainer.h>
#include <geParticle/StandardParticleAffectors.h>
#include <geParticle/PointEmitter.h>
#include "ComponentSystemRenderer.h"
#include "CustomTypes.h"
#include <geParticle/StandardParticleComponents.h>

#define MAX_PARTICLES 100

void ge::examples::ComponentSystemPSManager::initialize(std::shared_ptr<ge::gl::Context> glContext)
{
	auto pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::CPU_GPU);
	pc->registerComponent<ge::particle::LifeData>(false);
	pc->registerComponent<ge::particle::Position>(true);
	pc->registerComponent<ge::particle::Velocity>(false);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	auto lifeTimeAffector = std::make_shared<ge::particle::LifeTimeAffector>();
	ps->addAffector(lifeTimeAffector);

	auto linearMovementAffector = std::make_shared<ge::particle::LinearMovementAffector>();
	ps->addAffector(linearMovementAffector);

	ps->addAffector(std::make_shared<ge::particle::GravityAffector>());

	auto pointEmitter = std::make_shared<ge::particle::PointEmitter>(1, glm::vec3(-1, 0, 0));
	auto customFactory = std::make_shared<ge::particle::CSCustomFactory>();

	pointEmitter->setParticleFactory(customFactory);

	ps->addEmitter(pointEmitter);

	auto particleRenderer = std::make_shared<ge::particle::ComponentSystemRenderer>(glContext, MAX_PARTICLES, pc);
	ps->particleRenderer = particleRenderer;

	ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::examples::ComponentSystemPSManager::update()
{
	ps->update(ge::core::time_point::clock::now());
}
