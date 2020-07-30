#pragma once
#include "ExampleManager.h"
#include "geParticle/ParticleInitiator.h"
#include "geParticle/ParticleRenderer.h"
#include "geParticle/ParticleSystem.h"
#include "geParticle/ParticleAffector.h"
#include "geParticleGL/GPUParticleContainer.h"
#include "geParticleStd/ColorInitiator.h"
#include "geParticleStd/PointEmitter.h"
#include "geParticleStd/VelocityInitiator.h"

namespace ge
{
	namespace examples
	{	
		class SimpleExample : public Example
		{
		public:
			SimpleExample(std::shared_ptr<particle::ParticleSystemManager> &manager);
			void init() override;
			void render() override;
			void reset() override;
			void renderGui() override;
			std::string getName() const override { return "Simple Particle System"; }
			unsigned getContainerSize() const override;
			void stopEmitting() override;

		private:
			void initAffectors();
			void renderGuiAffectorDetails(std::string affectorName, std::shared_ptr<particle::ParticleAffector> ptr) const;
			void changeAffector(std::shared_ptr<particle::ParticleAffector> affector, bool active) const;
			
			std::shared_ptr<particle::ParticleSystemManager> manager;
			std::shared_ptr<particle::ParticleSystem> ps;
			std::shared_ptr<particle::GPUParticleContainer> pc;
			std::shared_ptr<particle::ParticleRenderer> renderer;

			std::shared_ptr<particle::PointEmitter> emitter;
			std::shared_ptr<particle::VelocityInitiator> velInitiator;
			std::shared_ptr<particle::ColorInitiator> colInitiator;

			std::vector<std::tuple<std::string, std::shared_ptr<particle::ParticleAffector>, bool>> allAffectors;
		};
	}
}
