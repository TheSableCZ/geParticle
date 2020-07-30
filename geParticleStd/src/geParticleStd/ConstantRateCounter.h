/** @file ConstantRateCounter.h
 *  @brief Generator of constant number per second.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geParticle/Counter.h>
#include <geParticleStd/Export.h>

namespace ge
{
    namespace particle
    {
    	/**
    	 * @brief Generator of constant number per second.
    	 */
		class GEPARTICLESTD_EXPORT ConstantRateCounter : public Counter
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
