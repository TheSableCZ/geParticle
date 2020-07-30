/** @file FireworkRenderer.h
 *  @brief Renderer for firework effect.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include "geParticle/ParticleRenderer.h"
#include "geParticleGL/GPUParticleContainer.h"

namespace ge
{
	namespace examples
	{
		class FireworkRenderer : public particle::ParticleRenderer
		{
		public:
			FireworkRenderer(std::shared_ptr<particle::GPUParticleContainer> container);
			void render(std::shared_ptr<particle::ParticleContainer> container) override;

		private:
			std::shared_ptr<ge::gl::Program> shaderProgram;
			std::shared_ptr<ge::gl::VertexArray> VAO;

			GLuint texture;
			GLuint textureUniformID;
		};
	}
}
