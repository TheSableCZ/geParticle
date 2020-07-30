/** @file ParticleSystemManager.cpp
 *  @brief Manager class for instances of ParticleSystem.
 *  @author Jan Sobol xsobol04
 */

#include <geParticle/ParticleSystemManager.h>
#include <algorithm>

void ge::particle::ParticleSystemManager::update(const core::time_point & t)
{
	if (!paused) {
		std::for_each(managed.begin(), managed.end(), [t](std::shared_ptr<ParticleSystem> &ps) { ps->update(t); });
	}
}

void ge::particle::ParticleSystemManager::registerParticleSystem(std::shared_ptr<ParticleSystem>& ps, const core::time_point & startTime)
{
	auto it = std::find(managed.begin(), managed.end(), ps);
	if (it == managed.end())
	{
		managed.push_back(ps);
	}
	ps->setStartTime(startTime);
}

void ge::particle::ParticleSystemManager::unregisterParticleSystem(std::shared_ptr<ParticleSystem>& ps)
{
	managed.remove(ps);
}

void ge::particle::ParticleSystemManager::startAll(const core::time_point & startTime)
{
	std::for_each(managed.begin(), managed.end(), [startTime](std::shared_ptr<ParticleSystem> &ps) { ps->setStartTime(startTime); });
	paused = false;
}

void ge::particle::ParticleSystemManager::pauseAll()
{
	paused = true;
}
