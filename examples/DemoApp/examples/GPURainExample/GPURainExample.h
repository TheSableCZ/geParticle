/** @file GPURainExample.h
 *  @brief GPU accelerated rain particle system.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include "ExampleManager.h"
#include "GPUPlaneEmitter.h"
#include "geParticle/ParticleRenderer.h"
#include "geParticle/ParticleSystem.h"
#include "geParticleGL/GPUParticleContainer.h"

namespace ge
{
	namespace examples
	{	
		class GPURainExample : public Example
		{
		public:
			GPURainExample(std::shared_ptr<particle::ParticleSystemManager> &manager);
			void init() override;
			void render() override;
			void reset() override;
			void renderGui() override;
			std::string getName() const override { return "GPU Rain"; }
			unsigned getContainerSize() const override;

		private:
			std::shared_ptr<particle::ParticleSystemManager> manager;
			std::shared_ptr<particle::ParticleSystem> ps;
			std::shared_ptr<particle::GPUParticleContainer> pc;
			std::shared_ptr<particle::ParticleRenderer> renderer;

			std::shared_ptr<GPUPlaneEmitter> emitter;
		};
	}
}