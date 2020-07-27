#pragma once

#include <geParticle/ParticleContainer.h>
#include <geParticleGL/GPUParticleEmitter.h>
#include <random>

namespace ge {
	namespace particle {
		class BallGPUEmitter : public GPUParticleEmitter 
		{
		public:
			BallGPUEmitter(std::string shaderSource, int particlesPerSecond, int maxParticles, int ballRadius);
			void emitParticles(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

		private:
			int ballRadius;
			std::shared_ptr<ge::gl::Buffer> randomBuffer;
			void refreshRandomBuffer(int count);
			std::mt19937 eng;
		};
	}
}
