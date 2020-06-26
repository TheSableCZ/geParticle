#pragma once

#include <geCore/Updatable.h>

namespace ge
{
	namespace particle
	{
		class Counter
		{
		public:
			virtual unsigned int getNumOfParticlesToCreate(core::time_unit dt) = 0;
		};
	}
}