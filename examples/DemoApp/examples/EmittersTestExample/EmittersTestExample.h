/** @file EmittersTestExample.h
 *  @brief Simple example to test standard emitters.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include "ExampleManager.h"
#include "geParticle/ParticleRenderer.h"
#include "geParticle/ParticleSystem.h"
#include "geParticleGL/GPUParticleContainer.h"
#include "geParticleStd/StandardEmitter.h"

namespace ge
{
	namespace examples
	{
		class EmittersTestExample : public Example
		{
		public:
			EmittersTestExample(std::shared_ptr<particle::ParticleSystemManager> &manager);
			void init() override;
			void render() override;
			void reset() override;
			void renderGui() override;
			std::string getName() const override { return "Standard Emitters Test"; }
			unsigned getContainerSize() const override;

		private:
			void initEmitters();
			void changeEmitter(std::shared_ptr<particle::StandardEmitter>& emitter) const;
			
			std::shared_ptr<particle::ParticleSystemManager> manager;
			std::shared_ptr<particle::ParticleSystem> ps;
			std::shared_ptr<particle::GPUParticleContainer> pc;
			std::shared_ptr<particle::ParticleRenderer> renderer;

			std::vector<std::pair<std::string, std::shared_ptr<particle::StandardEmitter>>> allEmitters;
			std::string selectedEmitter = "";
		};
	}
}
