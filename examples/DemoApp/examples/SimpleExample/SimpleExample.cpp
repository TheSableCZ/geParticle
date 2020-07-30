#include "SimpleExample.h"

#include <imgui.h>
#include <sstream>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticleStd/LinearMovementAffector.h>
#include <geParticleStd/GravityAffector.h>
#include <geParticleStd/LifeTimeAffector.h>
#include <geParticle/ParticleSystemManager.h>

#include "examples/Common/SimpleExampleRenderer.h"
#include "geParticleStd/ColorAffector.h"
#include "geParticleStd/FloorAffector.h"

constexpr auto MAX_PARTICLES = 500;

ge::examples::SimpleExample::SimpleExample(std::shared_ptr<particle::ParticleSystemManager>& manager)
	: manager(manager)
{}

void ge::examples::SimpleExample::init()
{
	pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::CPU_GPU, false, 50);
	pc->registerComponent<ge::particle::LifeData>(false);
	pc->registerComponent<ge::particle::Position>(true);
	pc->registerComponent<ge::particle::Velocity>(false);

	pc->registerComponent<particle::Color>(true);

	/*pc->setLiveParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
	{ return container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);
	pc->setDeadParticlePredicate(
		[](const int i, const ge::particle::ComponentSystemContainer &container) -> bool
	{ return !container.getComponent<ge::particle::LifeData>(i).livingFlag; }
	);*/

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

	initAffectors();

	emitter = std::make_shared<ge::particle::PointEmitter>(10, glm::vec3(0, -1, 0), core::time_unit(3.f));

	colInitiator = std::make_shared<particle::ColorInitiator>();
	emitter->initiators.push_back(colInitiator);

	velInitiator = std::make_shared<particle::VelocityInitiator>();
	emitter->initiators.push_back(velInitiator);
	ps->addEmitter(emitter);

	manager->registerParticleSystem(ps);

	renderer = std::make_shared<SimpleExampleRenderer>(pc);
}

void ge::examples::SimpleExample::render()
{
	renderer->render(pc);
}

void ge::examples::SimpleExample::reset()
{
	manager->unregisterParticleSystem(ps);
	ps.reset();
	pc.reset();
	allAffectors.clear();
	renderer.reset();
}

void ge::examples::SimpleExample::renderGui()
{
	ImGui::Begin(getName().c_str());
	if (ImGui::TreeNode("Emitter")) 
	{
		GuiRange(velInitiator->speedRange, "Speed", 0.f, 20.f);
		GuiVec3(velInitiator->maindir, "Main direction", -1.f, 1.f);
		ImGui::SliderFloat("Spread", &velInitiator->spread, 0.f, 5.f);

		float lifetime = emitter->getLife().count();
		if (ImGui::SliderFloat("Lifetime", &lifetime, 0.f, 6.f)) {
		    auto newTime = core::time_unit(lifetime);
            emitter->setLife(newTime);
        }

		int numOfParticles = std::static_pointer_cast<particle::ConstantRateCounter>(emitter->getRefCounter())->getParticlesPerSecond();
		if (ImGui::SliderInt("Particles/sec", &numOfParticles, 0, 100))
			std::static_pointer_cast<particle::ConstantRateCounter>(emitter->getRefCounter())->setParticlesPerSecond(numOfParticles);

		ImGui::Checkbox("Random color", &colInitiator->randomColor);
		ImGui::ColorEdit4("Particle color", &colInitiator->color[0]);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Affectors"))
	{
		for(auto & [name, ptr, active] : allAffectors)
		{
			if (ImGui::Checkbox(name.c_str(), &active)) changeAffector(ptr, active);
			renderGuiAffectorDetails(name, ptr);
		}
		
		ImGui::TreePop();
	}
	
	ImGui::End();
}

unsigned ge::examples::SimpleExample::getContainerSize() const
{
	return pc->size();
}

void ge::examples::SimpleExample::stopEmitting()
{
	std::static_pointer_cast<particle::ConstantRateCounter>(emitter->getRefCounter())->setParticlesPerSecond(0);
}

void ge::examples::SimpleExample::initAffectors()
{
	auto lifeTimeAffector = std::make_shared<ge::particle::LifeTimeAffector>();
	allAffectors.emplace_back(std::make_tuple("Lifetime Affector", lifeTimeAffector, true));

	auto linearMovementAffector = std::make_shared<ge::particle::LinearMovementAffector>();
	allAffectors.emplace_back(std::make_tuple("Linear Movement Affector", linearMovementAffector, true));

	auto gravityAffector = std::make_shared<ge::particle::GravityAffector>();
	allAffectors.emplace_back(std::make_tuple("Gravity Affector", gravityAffector, true));

	auto colorAffector = std::make_shared<particle::ColorAffector>();
	colorAffector->colorPoints.emplace_back(std::make_pair(0, glm::vec4(1.f, 0.f, 0.f, 1.f)));
	colorAffector->colorPoints.emplace_back(std::make_pair(100, glm::vec4(0.f, 0.f, 1.f, 1.f)));
	
	allAffectors.emplace_back(std::make_tuple("Color Affector", colorAffector, false));

	auto floorAffector = std::make_shared<particle::FloorAffector>();
	allAffectors.emplace_back(std::make_tuple("Floor Affector", floorAffector, false));

	for (auto &[name, ptr, active] : allAffectors)
	{
		if (active)
			ps->addAffector(ptr);
	}
}

void ge::examples::SimpleExample::renderGuiAffectorDetails(std::string affectorName, std::shared_ptr<ge::particle::ParticleAffector> ptr) const
{
	if (affectorName == "Color Affector")
	{
		auto colAffector = std::static_pointer_cast<particle::ColorAffector>(ptr);

		int cnt = 1;
		for (auto &[percentage, color] : colAffector->colorPoints)
		{
			std::stringstream ss;
			ss << cnt << ". " << "colorpoint";
			if (ImGui::TreeNode(ss.str().c_str()))
			{
				//ImGui::Text(ss.str().c_str());
				ImGui::SliderInt("%", &percentage, 0, 100);
				ImGui::ColorEdit4("Color", &color[0]);
				ImGui::TreePop();
			}

			cnt++;
		}

		if (ImGui::Button("Add Colorpoint"))
		{
			colAffector->colorPoints.emplace_back(
				std::make_pair(
					colAffector->colorPoints.back().first,
					colAffector->colorPoints.back().second
				)
			);
		}
	}

	if (affectorName == "Floor Affector")
	{
		ImGui::SliderFloat("Position Y", &std::static_pointer_cast<particle::FloorAffector>(ptr)->axisY, -10.f, 10.f);
		ImGui::SliderFloat("Bounce", &std::static_pointer_cast<particle::FloorAffector>(ptr)->resilience, 0.f, 1.f);
		ImGui::SliderFloat("Friction", &std::static_pointer_cast<particle::FloorAffector>(ptr)->friction, 0.f, 1.f);
		ImGui::SliderFloat("Cutoff", &std::static_pointer_cast<particle::FloorAffector>(ptr)->cutoff, 0.f, 1.f);
		ImGui::SliderFloat("Hit Tolerance", &std::static_pointer_cast<particle::FloorAffector>(ptr)->floorHitTolerance, 0.f, 1.f);
	}
}

void ge::examples::SimpleExample::changeAffector(std::shared_ptr<particle::ParticleAffector> affector, bool active) const
{
	if (active)
	{
		ps->addAffector(affector);
	}
	else
	{
		ps->removeAffector(affector);
	}
}
