﻿#include "FireworkExample.h"



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
#include "geParticleStd/PointEmitter.h"
#include "geParticleStd/StandardParticleComponents.h"
#include "geParticleStd/VelocityInitiator.h"

// original 1000
constexpr auto MAX_PARTICLES = 10000;

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

	auto it = pc->begin<particle::LifeData>();
	pc->setLiveParticlePredicate(
		[it](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
	//{ return container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	{ it->setIndex(i); return it->get().livingFlag; }
	);
	pc->setDeadParticlePredicate(
		[it](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
	//{ return !container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	{ it->setIndex(i); return !it->get().livingFlag; }
	);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	// Plane emitter 4x4 worldspace
	auto plane = std::make_shared<particle::BoxEmitter>(-12, 12, -2, -2, -2, 2, core::time_unit(1.f), std::make_shared<particle::ConstantRateCounter>(10));
	plane->initiators.emplace_back(std::make_shared<particle::VelocityInitiator>(glm::vec2(9, 12)));
	plane->initiators.emplace_back(std::make_shared<particle::ColorInitiator>());
	plane->initiators.emplace_back(std::make_shared<FireworkAttribInitiator>());

	ps->addEmitter(plane);

	auto velInit = std::make_shared<particle::VelocityInitiator>(glm::vec2(8, 9), 3.f);
	auto colInit = std::make_shared<particle::ColorInitiator>();
	colInit->randomColor = false;
	colInit->color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	auto fountainInit = std::make_shared<FountainInitiator>();
	for (int i = 0; i < 5; i++)
	{
		auto pointEm = std::make_shared<particle::PointEmitter>(2, glm::vec3((float)i * 5 - 10.f, -5, 10), core::time_unit(1.f));
		pointEm->initiators.emplace_back(velInit);
		pointEm->initiators.emplace_back(colInit);
		pointEm->initiators.emplace_back(fountainInit);

		ps->addEmitter(pointEm);
	}

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
	renderer.reset();
}

void ge::examples::FireworkExample::renderGui()
{
}
