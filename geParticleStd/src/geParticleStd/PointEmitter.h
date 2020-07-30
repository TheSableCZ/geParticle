/** @file PointEmitter.h
 *  @brief One point emitter.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <geParticleStd/StandardEmitter.h>

namespace ge {
	namespace particle {

		/**
		 * @brief One point emitter.
		 */
		class GEPARTICLESTD_EXPORT PointEmitter : public StandardEmitter
		{
			class PointInitiator : public ParticleInitiator
			{
			public:
				PointInitiator(glm::vec3 pos) : pos(pos) {}

				void init(std::shared_ptr<RangeParticleContainerIterator> range) override;

			protected:
				glm::vec3 pos;
			};

		public:
			PointEmitter(int particlesPerSecond, glm::vec3 pos, core::time_unit life);
		};
	}
}
