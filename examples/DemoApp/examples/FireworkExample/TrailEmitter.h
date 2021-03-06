﻿/** @file TrailEmitter.h
 *  @brief Firework trail emitter.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include "geParticle/ParticleEmitter.h"
#include "geParticle/ParticleInitiator.h"

namespace ge
{
	namespace examples
	{
		class TrailEmitter : public particle::ParticleEmitter
		{
		public:
			TrailEmitter();
			void emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

			void addParticle(glm::vec3 pos, glm::vec4 color);

			std::vector<std::shared_ptr<particle::ParticleInitiator>> initiators;

		protected:
			std::vector<glm::vec3> posQueue;
			std::vector<glm::vec4> colorQueue;
		};
	}
}
