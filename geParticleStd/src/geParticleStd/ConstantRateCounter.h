#pragma once

#include <geParticle/Counter.h>

namespace ge
{
    namespace particle
    {
		class ConstantRateCounter : public Counter
		{
		public:
			ConstantRateCounter(int particlesPerSecond)
				: particlesPerSecond(particlesPerSecond)
			{}

			unsigned int getNum(core::time_unit dt) override;

			void setParticlesPerSecond(int particlesPerSecond) { this->particlesPerSecond = particlesPerSecond; }
			int getParticlesPerSecond() const { return particlesPerSecond; }
			
		protected:
			int particlesPerSecond;
			double carryOver = .0f;
		};
    }
}
