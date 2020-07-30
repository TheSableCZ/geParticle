#pragma once

#include <memory>
#include <list>
#include <geCore/Updatable.h>
#include <geParticle/ParticleSystem.h>

namespace ge
{
    namespace particle
    {
		class ParticleSystemManager : public ge::core::Updatable {
		public:
			virtual ~ParticleSystemManager() = default;
			void update(const core::time_point& t) override;
			void registerParticleSystem(std::shared_ptr<ParticleSystem>& ps, const core::time_point& startTime = core::time_point());
			void unregisterParticleSystem(std::shared_ptr<ParticleSystem>& ps);
			void startAll(const core::time_point& startTime);
			void pauseAll();

		protected:
			std::list<std::shared_ptr<ParticleSystem>> managed;
			bool paused = true;
		};
    }
}
