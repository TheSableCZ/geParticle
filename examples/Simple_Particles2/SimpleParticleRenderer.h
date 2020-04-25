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
		class SimpleParticleRenderer : public ParticleRenderer {
		public:
			SimpleParticleRenderer(std::shared_ptr<ge::gl::Context> glContext, int maxParticles);
			void render(std::shared_ptr<ParticleContainer> container) override;

		protected:
			int maxParticles;

			std::shared_ptr<ge::gl::Context> gl;
			std::shared_ptr<ge::gl::Program> shaderProgram;
			std::shared_ptr<ge::gl::VertexArray> VAO;

			std::shared_ptr<ge::gl::Buffer> billboardVertices;
			std::shared_ptr<ge::gl::Buffer> particleCenters;
			std::shared_ptr<ge::gl::Buffer> particleColors;
			std::vector<float> centers;
			std::vector<float> colors;

			void initialize();
		};
	}
}
