#pragma once

#include <geParticle/ParticleEmitter.h>

namespace ge
{
    namespace particle
    {

		class ConstantRateEmitter : public ParticleEmitterBase
		{
		public:
			ConstantRateEmitter(int particlesPerSecond)
				: particlesPerSecond(particlesPerSecond)
			{}

		protected:
			unsigned int getNumOfParticlesToCreate(core::time_unit dt) {
				double realValue = (dt.count() * (float)particlesPerSecond);

				int newParticles = (int)realValue;
				carryOver += realValue - (double)newParticles;
				if (carryOver > 1.0) {
					newParticles++;
					carryOver -= 1.0;
				}

				return newParticles;
			}

			int particlesPerSecond;
			double carryOver = .0f;
		};
    }
}
