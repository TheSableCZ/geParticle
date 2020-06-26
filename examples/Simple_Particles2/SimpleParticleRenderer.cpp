#include "SimpleParticleRenderer.h"
#include <geGL/geGL.h>
#include <geUtil/Text.h>
//#include <geParticle/ParticleIterator.h>
#include <iostream>
#include "BillboardVertices.h"
#include "Camera.h"
#include "CustomTypes.h"
#include <geParticle/ArrayOfStructsContainer.h>

ge::particle::SimpleParticleRenderer::SimpleParticleRenderer(std::shared_ptr<ge::gl::Context> glContext, int maxParticles)
	: gl(glContext), maxParticles(maxParticles)
{
	initialize();
}

void ge::particle::SimpleParticleRenderer::initialize()
{
	std::cout << "OpenGL version:\n  " << gl->glGetString(GL_VERSION) << std::endl;

	std::string vexShd = ge::util::loadTextFile(VERTEX_SHADER);
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, vexShd);
	std::string frgShd = ge::util::loadTextFile(FRAGMENT_SHADER);
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, frgShd);
	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);

	billboardVertices = std::make_shared<ge::gl::Buffer>(particleVertices.size() * sizeof(float), particleVertices.data());

	particleCenters = std::make_shared<ge::gl::Buffer>(maxParticles * sizeof(float) * 3);
	centers = std::vector<float>();

	particleColors = std::make_shared<ge::gl::Buffer>(maxParticles * sizeof(float) * 3);

	VAO = std::make_shared<ge::gl::VertexArray>();
	VAO->addAttrib(billboardVertices, 0, 3, GL_FLOAT);
	VAO->addAttrib(particleCenters, 1, 3, GL_FLOAT);
	VAO->addAttrib(particleColors, 2, 3, GL_FLOAT);
}

void ge::particle::SimpleParticleRenderer::render(std::shared_ptr<ParticleContainer> container)
{
	centers.clear();
	colors.clear();
	int particleCount = 0;

	if (container->getType() == ParticleContainerType::AoS) {
		auto pi = std::static_pointer_cast<ArrayOfStructsContainer<Particle>::iterator>(container->begin());
		auto end = container->end();
		for (; *pi != *end; (*pi)++) {
			auto &p = static_cast<CustomParticle &>(**pi);
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
}
