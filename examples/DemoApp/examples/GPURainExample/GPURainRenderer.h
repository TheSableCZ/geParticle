#pragma once
#include "geParticle/ParticleRenderer.h"
#include "geParticleGL/GPUParticleContainer.h"

namespace ge
{
	namespace examples
	{
		class GPURainRenderer : public particle::ParticleRenderer
		{
		public:
			GPURainRenderer(std::shared_ptr<particle::GPUParticleContainer> container);
			void render(std::shared_ptr<particle::ParticleContainer> container) override;

		private:
			std::shared_ptr<ge::gl::Program> shaderProgram;
			std::shared_ptr<ge::gl::VertexArray> VAO;

			GLuint texture;
			GLuint textureUniformID;
		};
	}
}
