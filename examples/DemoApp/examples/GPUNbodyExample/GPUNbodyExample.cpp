/** @file GPUNbodyExample.cpp
 *  @brief N-body simulation example.
 *  @author Jan Sobol xsobol04
 */

#include "GPUNbodyExample.h"

#include <random>

#include "GPUNbodyRenderer.h"
#include "geParticle/ParticleSystemManager.h"
#include "geParticleGL/GPUParticleAffector.h"

ge::examples::GPUNbodyExample::GPUNbodyExample(std::shared_ptr<particle::ParticleSystemManager>& manager)
	: manager(manager)
{
}

constexpr int MAX_PARTICLES = 4096;

void ge::examples::GPUNbodyExample::init()
{
	pc = std::make_shared<ge::particle::GPUParticleContainer>(MAX_PARTICLES, ge::particle::GPUParticleContainer::GPU_ONLY);

	pc->registerComponent<ge::particle::GPUMassPointData>(false, distributeParticles());
	pc->bindComponentBase<ge::particle::GPUMassPointData>(0);

	pc->registerComponent<ge::particle::Color>(false, randomColors());

	ps = std::make_shared<ge::particle::ParticleSystem>(pc);

	const std::string affectorSource =
#include "GPUNbody/affector.glsl"
		;
	auto affector = std::make_shared<ge::particle::GPUParticleAffector>(affectorSource);
	ps->addAffector(affector);

	manager->registerParticleSystem(ps);

	renderer = std::make_shared<GPUNbodyRenderer>(pc);
}

void ge::examples::GPUNbodyExample::render()
{
	renderer->render(pc);
}

void ge::examples::GPUNbodyExample::reset()
{
	manager->unregisterParticleSystem(ps);
	ps.reset();
	pc.reset();
	renderer.reset();
}

void ge::examples::GPUNbodyExample::renderGui()
{
	ImGui::Begin(getName().c_str());

	ImGui::End();
}

unsigned ge::examples::GPUNbodyExample::getContainerSize() const
{
	return pc->size();
}

std::vector<ge::particle::GPUMassPointData> ge::examples::GPUNbodyExample::distributeParticles()
{
	std::vector<ge::particle::GPUMassPointData> vect;

	float rndX, rndY, rndZ;
	auto fInitRadius = static_cast<float>(30);
	std::mt19937 eng;
	std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

	for (auto i = 0u; i < MAX_PARTICLES; ++i) {
		rndX = dist(eng);
		rndY = dist(eng);
		rndZ = dist(eng);

		ge::particle::GPUMassPointData item;
		item.position = glm::vec4(rndX, rndY, 0.f /*rndZ*/, 1.f);
		item.velocity = glm::vec4(0.f);

		vect.push_back(item);
	}

	return vect;
}

std::vector<ge::particle::Color> ge::examples::GPUNbodyExample::randomColors()
{
	std::vector<particle::Color> vect(MAX_PARTICLES);
	const glm::vec4 colors[] = {
		glm::vec4(1.f, 0.f, 0.f, 1.f),
		glm::vec4(0.f, 1.f, 0.f, 1.f),
		glm::vec4(0.f, 0.f, 1.f, 1.f) };

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		vect[i] = particle::Color{ colors[rand() % 3] };
	}

	return vect;
}
