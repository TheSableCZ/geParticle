/** @file ConstantRateCounter.cpp
 *  @brief Generator of constant number per second.
 *  @author Jan Sobol xsobol04
 */

#include <geParticleStd/ConstantRateCounter.h>

unsigned int ge::particle::ConstantRateCounter::getNum(core::time_unit dt)
{
	double realValue = (dt.count() * (float)particlesPerSecond);

	int newParticles = (int)realValue;
	carryOver += realValue - (double)newParticles;
	if (carryOver > 1.0) {
		newParticles++;
		carryOver -= 1.0;
	}

	return newParticles;
}
