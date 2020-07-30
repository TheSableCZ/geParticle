/** @file Counter.h
 *  @brief Abstract class for object that generates numbers.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geCore/Updatable.h>

namespace ge
{
	namespace particle
	{
		/**
		 *  @brief Abstract class for object that generates numbers.
		 */
		class Counter
		{
		public:
			virtual ~Counter() = default;

			/**
			 * @brief Generate and get next number.
			 * @param dt Delta time.
			 */
			virtual unsigned int getNum(core::time_unit dt) = 0;
		};
	}
}