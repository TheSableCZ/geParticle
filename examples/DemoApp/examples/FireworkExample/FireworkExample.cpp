#include "FireworkExample.h"



#include "ExplosionAffector.h"
#include "ExplosionEmitter.h"
#include "FireworkRenderer.h"
#include "TrailAffector.h"
#include "TrailEmitter.h"
#include "geParticle/ParticleSystemManager.h"
#include "geParticleStd/BoxEmitter.h"
#include "geParticleStd/ColorInitiator.h"
#include "geParticleStd/LifeTimeAffector.h"
#include "geParticleStd/LinearMovementAffector.h"
#include "geParticleStd/StandardParticleComponents.h"
#include "geParticleStd/VelocityInitiator.h"

constexpr auto MAX_PARTICLES = 1000;

ge::examples::FireworkExample::FireworkExample(std::shared_ptr<particle::ParticleSystemManager>& manager)
	: manager(manager)
{
}

void ge::examples::FireworkExample::init()
{
	pc = std::make_shared<particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::CPU_GPU);
	pc->registerComponent<particle::LifeData>(false);
	pc->registerComponent<particle::Position>(true);
	pc->registerComponent<particle::Velocity>(false);
	pc->registerComponent<particle::Color>(true);
	pc->registerComponent<OrigColor>(false);

	pc->registerComponent<Type>(false);
	pc->registerComponent<ExplodedFlag>(false);
	pc->registerComponent<Size>(true);

	pc->setLiveParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
	{ return container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);
	pc->setDeadParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
	{ return !container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	// Plane emitter 4x4 worldspace
	auto plane = std::make_shared<particle::BoxEmitter>(-2, 2, -2, -2, -2, 2, core::time_unit(1.f), std::make_shared<particle::ConstantRateCounter>(1));
	plane->initiators.emplace_back(std::make_shared<particle::VelocityInitiator>(glm::vec2(8, 10)));
	plane->initiators.emplace_back(std::make_shared<particle::ColorInitiator>());
	plane->initiators.emplace_back(std::make_shared<FireworkAttribInitiator>());

	ps->addEmitter(plane);

	ps->addAffector(std::make_shared<particle::LinearMovementAffector>());
	ps->addAffector(std::make_shared<TypedGravityAffector>());
	ps->addAffector(std::make_shared<particle::LifeTimeAffector>());
	ps->addAffector(std::make_shared<TypedColorAffector>());
	ps->addAffector(std::make_shared<SizeAffector>());

	auto explosionEmitter = std::make_shared<ExplosionEmitter>();
	ps->addEmitter(explosionEmitter);
	ps->addAffector(std::make_shared<ExplosionAffector>(explosionEmitter));

	auto trailEmitter = std::make_shared<TrailEmitter>();
	ps->addEmitter(trailEmitter);
	ps->addAffector(std::make_shared<TrailAffector>(trailEmitter));

	manager->registerParticleSystem(ps);

	renderer = std::make_shared<FireworkRenderer>(pc);
}

void ge::examples::FireworkExample::render()
{
	renderer->render(pc);
}

void ge::examples::FireworkExample::reset()
{
	manager->unregisterParticleSystem(ps);
	ps.reset();
	pc.reset();
}

void ge::examples::FireworkExample::renderGui()
{
}
