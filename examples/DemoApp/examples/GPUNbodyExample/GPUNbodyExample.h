#pragma once
#include "ExampleManager.h"
#include "geParticle/ParticleRenderer.h"
#include "geParticle/ParticleSystem.h"
#include "geParticleGL/GPUParticleContainer.h"
#include "geParticleStd/StandardParticleComponents.h"

namespace ge
{
	namespace examples
	{
		class GPUNbodyExample : public Example
		{
		public:
			GPUNbodyExample(std::shared_ptr<particle::ParticleSystemManager> &manager);
			void init() override;
			void render() override;
			void reset() override;
			void renderGui() override;
			std::string getName() const override { return "GPU N-body Simulation"; }
			unsigned getContainerSize() const override;

		private:
			std::vector<ge::particle::GPUMassPointData> distributeParticles();
			std::vector<particle::Color> randomColors();
			
			std::shared_ptr<particle::ParticleSystemManager> manager;
			std::shared_ptr<particle::ParticleSystem> ps;
			std::shared_ptr<particle::GPUParticleContainer> pc;
			std::shared_ptr<particle::ParticleRenderer> renderer;
		};
	}
}
