/** @file EmittersTestExample.cpp
 *  @brief Simple example to test standard emitters.
 *  @author Jan Sobol xsobol04
 */

#include "EmittersTestExample.h"

#include "examples/Common/SimpleExampleRenderer.h"
#include "geParticle/ParticleSystemManager.h"
#include "geParticleStd/BallEmitter.h"
#include "geParticleStd/BoxEmitter.h"
#include "geParticleStd/ColorInitiator.h"
#include "geParticleStd/PointEmitter.h"
#include "geParticleStd/StandardParticleComponents.h"

constexpr auto MAX_PARTICLES = 1000;

ge::examples::EmittersTestExample::EmittersTestExample(std::shared_ptr<particle::ParticleSystemManager>& manager)
	: manager(manager)
{
}

void ge::examples::EmittersTestExample::init()
{
	pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::CPU_GPU);
	pc->registerComponent<ge::particle::LifeData>(false);
	pc->registerComponent<ge::particle::Position>(true);
	pc->registerComponent<ge::particle::Velocity>(false);

	pc->registerComponent<particle::Color>(true);

	pc->setLiveParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
		{ return container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);
	pc->setDeadParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
	{ return !container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	manager->registerParticleSystem(ps);

	renderer = std::make_shared<SimpleExampleRenderer>(pc);

	initEmitters();
}

void ge::examples::EmittersTestExample::render()
{
	renderer->render(pc);
}

void ge::examples::EmittersTestExample::reset()
{
	manager->unregisterParticleSystem(ps);
	ps.reset();
	pc.reset();
	allEmitters.clear();
	renderer.reset();
}

void ge::examples::EmittersTestExample::renderGui()
{
	ImGui::Begin(getName().c_str());
	if (ImGui::TreeNode("Emitter"))
	{
		if (ImGui::BeginCombo("change emitter", selectedEmitter.c_str()))
		{
			for (auto &pair : allEmitters)
			{
				if (ImGui::Selectable(pair.first.c_str()))
				{
					changeEmitter(pair.second);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

unsigned ge::examples::EmittersTestExample::getContainerSize() const
{
	return pc->size();
}

void ge::examples::EmittersTestExample::initEmitters()
{
    auto lifeTime = core::time_unit(1.f);
	allEmitters.emplace_back(std::make_pair("Point Emitter", std::make_shared<particle::PointEmitter>(10, glm::vec3(0, 0, 0), lifeTime)));
	allEmitters.emplace_back(std::make_pair("Box Emitter", std::make_shared<particle::BoxEmitter>(
		-1, 1, -1, 1, -1, 1, 
		lifeTime,
		std::make_shared<particle::ConstantRateCounter>(10)
		)));
	allEmitters.emplace_back(std::make_pair("Ball Emitter", std::make_shared<particle::BallEmitter>(
		glm::vec3(0, 0, 0), 1.f,
		lifeTime,
		std::make_shared<particle::ConstantRateCounter>(10)
		)));

	for (auto &pair : allEmitters)
	{
		pair.second->initiators.emplace_back(std::make_shared<particle::ColorInitiator>());
	}
}

void ge::examples::EmittersTestExample::changeEmitter(std::shared_ptr<particle::StandardEmitter>& emitter) const
{
	ps->particleEmitters.clear();
	ps->addEmitter(emitter);
}
