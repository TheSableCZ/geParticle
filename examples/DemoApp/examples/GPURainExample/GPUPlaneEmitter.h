/** @file GPUPlaneEmitter.h
 *  @brief GPU emitter with random particle distribution.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <random>

#include "geParticleGL/GPUParticleEmitter.h"

namespace ge
{
	namespace examples
	{
		class GPUPlaneEmitter : public particle::GPUParticleEmitter
		{
		public:
			GPUPlaneEmitter(std::string shaderSource, int particlesPerSecond, int bufferSize, glm::vec2 planeDimensions, float axisY);
			void emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

		protected:
			void refreshRandomBuffer(int count);
			
			int bufferSize;
			glm::vec2 dimensions;
			float axisY;
			std::shared_ptr<ge::gl::Buffer> randomBuffer;
			std::mt19937 generator;
		};
	}
}
