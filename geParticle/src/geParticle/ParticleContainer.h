/** @file ParticleContainer.h
 *  @brief Abstract class representing particle storage.
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geParticle/Particle.h>
#include <geParticle/ParticleContainerType.h>
#include <geParticle/ParticleContainerIterator.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Abstract class representing particle storage.
		 */
		class ParticleContainer {
		public:
			virtual ~ParticleContainer() = default;

			/**
			 * @brief For recognising storage type. Useful for static_pointer_cast.
			 * @return Container type.
			 */
			virtual ParticleContainerType getType() = 0;

			/**
			 * @brief Actual container size getter.
			 * @return Container size.
			 */
			virtual unsigned int size() = 0;

			virtual std::shared_ptr<ParticleContainerIterator> begin() = 0;
			virtual std::shared_ptr<ParticleContainerIterator> end() = 0;

			/**
			 * @brief Special iterator for finding unused particles.
			 */
			virtual std::shared_ptr<ParticleContainerIterator> getUnusedParticlesIterator() = 0;

			/**
			 * @brief Custom range iterator. Going through defined indexes.
			 */
			virtual std::shared_ptr<RangeParticleContainerIterator> createRangeIterator() = 0;
		};

		class IndexBasedParticleContainer : public ParticleContainer {
		public:
			virtual int startIdx() = 0;
			virtual int endIdx() = 0;
		};

	}
}
