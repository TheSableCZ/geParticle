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

			unsigned int getNumOfParticlesToCreate(core::time_unit dt) override;

			int particlesPerSecond;
			double carryOver = .0f;
		};
    }
}
