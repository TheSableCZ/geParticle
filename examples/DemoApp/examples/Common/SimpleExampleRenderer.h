#pragma once
#include <geParticle/ParticleRenderer.h>
#include <geParticleGL/GPUParticleContainer.h>

namespace ge
{
	namespace gl
	{
		class Context;
		class Program;
		class VertexArray;
		class Buffer;
	}
	
	namespace examples
	{
		class SimpleExampleRenderer : public particle::ParticleRenderer
		{
		public:
			SimpleExampleRenderer(std::shared_ptr<particle::GPUParticleContainer> container);
			void render(std::shared_ptr<particle::ParticleContainer> container) override;

		protected:
			//int maxParticles;

			//std::shared_ptr<ge::gl::Context> gl;
			std::shared_ptr<ge::gl::Program> shaderProgram;
			std::shared_ptr<ge::gl::VertexArray> VAO;

			std::shared_ptr<ge::gl::Buffer> billboardVertices;
			std::shared_ptr<ge::gl::Buffer> particleCenters;
			std::shared_ptr<ge::gl::Buffer> particleColors;
			std::vector<float> centers;
			std::vector<float> colors;
		};
	}
}
