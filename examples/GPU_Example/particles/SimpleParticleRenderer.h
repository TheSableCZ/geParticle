#pragma once

#include <geParticle/ParticleSystem.h>
#include <geParticle/ParticleRenderer.h>

namespace ge {
	namespace gl
	{
		class Context;
		class Program;
		class VertexArray;
		class Buffer;
	}

	namespace particle {
		class GPUParticleContainer;

		class SimpleParticleRenderer : public ParticleRenderer {
		public:
			SimpleParticleRenderer(std::shared_ptr<GPUParticleContainer> container);
			void render(std::shared_ptr<ParticleContainer> container) override;

		protected:
			std::shared_ptr<ge::gl::Program> shaderProgram;
			std::shared_ptr<ge::gl::VertexArray> VAO;

			void initialize(std::shared_ptr<GPUParticleContainer> container);
		};
	}
}
