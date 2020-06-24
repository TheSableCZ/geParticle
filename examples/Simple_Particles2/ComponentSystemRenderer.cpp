#include "ComponentSystemRenderer.h"
#include <geGL/geGL.h>
#include <geUtil/Text.h>
#include <iostream>
#include "BillboardVertices.h"
#include "Camera.h"
#include "CustomTypes.h"

ge::particle::ComponentSystemRenderer::ComponentSystemRenderer(std::shared_ptr<ge::gl::Context> glContext, int maxParticles, std::shared_ptr<GPUParticleContainer> container)
	: gl(glContext), maxParticles(maxParticles)
{
	initialize(container);
}

void ge::particle::ComponentSystemRenderer::initialize(std::shared_ptr<GPUParticleContainer> container)
{
	std::cout << "OpenGL version:\n  " << gl->glGetString(GL_VERSION) << std::endl;

	std::string vexShd = ge::util::loadTextFile(VERTEX_SHADER);
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, vexShd);
	std::string frgShd = ge::util::loadTextFile(FRAGMENT_SHADER);
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, frgShd);
	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);

	
	billboardVertices = std::make_shared<ge::gl::Buffer>(particleVertices.size() * sizeof(float), particleVertices.data());

	/*
	particleCenters = std::make_shared<ge::gl::Buffer>(maxParticles * sizeof(float) * 3);
	centers = std::vector<float>();
	*/

	/*
	particleColors = std::make_shared<ge::gl::Buffer>(maxParticles * sizeof(float) * 3);
	*/

	VAO = std::make_shared<ge::gl::VertexArray>();
	VAO->addAttrib(billboardVertices, 0, 3, GL_FLOAT);
	//VAO->addAttrib(particleCenters, 1, 3, GL_FLOAT);
	//VAO->addAttrib(particleColors, 2, 3, GL_FLOAT);

	container->addComponentVertexAttrib<Position>(VAO, 1, 3, GL_FLOAT, sizeof(Position), offsetof(Position, position));
	container->addComponentVertexAttrib<Color>(VAO, 2, 3, GL_FLOAT, sizeof(Color), offsetof(Color, color));
}

void ge::particle::ComponentSystemRenderer::render(std::shared_ptr<ParticleContainer> container)
{
	auto gpuContainer = std::static_pointer_cast<GPUParticleContainer>(container);

	auto particlesCount = gpuContainer->syncOnlyAlive(GPUParticleContainer::CPU_TO_GPU);

	shaderProgram->setMatrix4fv("MVP", &(examples::Camera::getInstance().getMVP())[0][0]);
	shaderProgram->set3fv("CameraUp", &(examples::Camera::getInstance().getCameraUp())[0]);
	shaderProgram->set3fv("CameraRight", &(examples::Camera::getInstance().getCameraRight())[0]);

	shaderProgram->use();
	VAO->bind();

	gl->glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	gl->glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	gl->glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	gl->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);

	/*centers.clear();
	colors.clear();
	int particleCount = 0;

	if (container->getType() == ParticleContainer::AoS) {
		auto pi = AoSParticleIterator(std::static_pointer_cast<ArrayOfStructsContainer>(container));
		for(pi; !pi.end(); pi.doNext()) {
			auto &p = static_cast<CustomParticle &>(pi.getInstance());
			if (p.livingFlag) {
				centers.push_back(p.pos.x);
				centers.push_back(p.pos.y);
				centers.push_back(p.pos.z);

				colors.push_back(p.color.r);
				colors.push_back(p.color.g);
				colors.push_back(p.color.b);

				particleCount++;
			}
		}
	}
	else {
		return;
	}

	shaderProgram->setMatrix4fv("MVP", &(examples::Camera::getInstance().getMVP())[0][0] );
	shaderProgram->set3fv("CameraUp", &(examples::Camera::getInstance().getCameraUp())[0] );
	shaderProgram->set3fv("CameraRight", &(examples::Camera::getInstance().getCameraRight())[0] );

	shaderProgram->use();
	VAO->bind();

	particleCenters->setData(centers);
	particleColors->setData(colors);

	gl->glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	gl->glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	gl->glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	gl->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);

	//std::cout << particleCount << " -- " << particleCenters->getSize() << "\n";
	*/
}
