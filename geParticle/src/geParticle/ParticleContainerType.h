/** @file ParticleContainerType.h
 *  @brief Enumeration containing two standard particle container types.
 *  @author Jan Sobol xsobol04
 */

#pragma once

namespace ge
{
    namespace particle
    {
    	/**
    	 * @brief Enumeration containing two standard particle container types.
    	 */
		enum class ParticleContainerType {
			AoS, SoA_CS
		};
    }
}
