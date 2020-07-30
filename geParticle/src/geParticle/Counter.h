#pragma once

#include <geCore/Updatable.h>

namespace ge
{
	namespace particle
	{
		class Counter
		{
		public:
			virtual ~Counter() = default;
			virtual unsigned int getNum(core::time_unit dt) = 0;
		};
	}
}