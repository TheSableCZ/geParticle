#pragma once
#include <geParticle/ParticleAffector.h>
#include <geParticle/ComponentSystemContainer.h>
#include <geParticleStd/StandardParticleComponents.h>

namespace ge
{
	namespace particle
	{
		class ColorAffector : public ParticleAffector
		{
		public:
			void affect(core::time_unit dt, std::shared_ptr<particle::ParticleContainer> particles) override;

			std::vector<std::pair<int, glm::vec4>> colorPoints;

		protected:
			glm::vec4 getColor(core::time_unit life, core::time_unit totalLifeTime) const;
		};
	}	
}
