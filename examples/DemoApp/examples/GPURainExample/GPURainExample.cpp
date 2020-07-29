#include "GPURainExample.h"

#include <imgui.h>
#include <geParticleStd/StandardParticleComponents.h>
#include <geParticle/ParticleSystemManager.h>

#include "GPURainRenderer.h"
#include "geParticleGL/GPUParticleAffector.h"


constexpr auto MAX_PARTICLES = 10000;

ge::examples::GPURainExample::GPURainExample(std::shared_ptr<particle::ParticleSystemManager>& manager)
	: manager(manager)
{}

void ge::examples::GPURainExample::init()
{
	pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::GPU_ONLY);

	pc->registerComponent<ge::particle::GPUMassPointData>();
	pc->registerComponent<ge::particle::GPULifeData>();

	pc->bindComponentBase<ge::particle::GPUMassPointData>(0);
	pc->bindComponentBase<ge::particle::GPULifeData>(1);

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	const std::string affectorSource =
#include "GPURain/affector.glsl"
		;
	auto affector = std::make_shared<ge::particle::GPUParticleAffector>(affectorSource);
	ps->addAffector(affector);

	const std::string emitterSource =
#include "GPURain/emitter.glsl"
		;
	emitter = std::make_shared<GPUPlaneEmitter>(emitterSource, 500, MAX_PARTICLES, glm::vec2(50, 50), 15.f);
	ps->addEmitter(emitter);

	manager->registerParticleSystem(ps);

	renderer = std::make_shared<GPURainRenderer>(pc);
}

void ge::examples::GPURainExample::render()
{
	renderer->render(pc);
}

void ge::examples::GPURainExample::reset()
{
	manager->unregisterParticleSystem(ps);
	ps.reset();
	pc.reset();
	renderer.reset();
}

void ge::examples::GPURainExample::renderGui()
{
	ImGui::Begin(getName().c_str());
	
	int numOfParticles = std::static_pointer_cast<particle::ConstantRateCounter>(emitter->getRefCounter())->getParticlesPerSecond();
	if (ImGui::SliderInt("Particles/sec", &numOfParticles, 100, 1000))
		std::static_pointer_cast<particle::ConstantRateCounter>(emitter->getRefCounter())->setParticlesPerSecond(numOfParticles);

	ImGui::End();
}
