#include "SimpleParticleRenderer.h"
#include <geGL/geGL.h>
#include <geUtil/Text.h>
#include <iostream>
#include "utils/Camera.h"
#include <geParticle/GPUParticleContainer.h>


ge::particle::SimpleParticleRenderer::SimpleParticleRenderer(std::shared_ptr<GPUParticleContainer> container)
{
	initialize(container);
}


void ge::particle::SimpleParticleRenderer::initialize(std::shared_ptr<GPUParticleContainer> container)
{
	std::string vexShd = ge::util::loadTextFile(APP_RESOURCES"/shaders/vertexShader.glsl");
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, vexShd);
	std::string frgShd = ge::util::loadTextFile(APP_RESOURCES"/shaders/fragmentShader.glsl");
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, frgShd);
	std::string gShd = ge::util::loadTextFile(APP_RESOURCES"/shaders/billboardGeometryShader.glsl");
	std::shared_ptr<ge::gl::Shader> geometryShader = std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, gShd);

	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader, geometryShader);

	VAO = std::make_shared<ge::gl::VertexArray>();

	container->addComponentAttrib<MassPointData>(VAO, 0, 3, GL_FLOAT, sizeof(MassPointData), offsetof(MassPointData, position));
}

void ge::particle::SimpleParticleRenderer::render(std::shared_ptr<ParticleContainer> container)
{
	shaderProgram->setMatrix4fv("MVP", &(examples::Camera::getInstance().getMVP())[0][0] );
	shaderProgram->set3fv("CameraUp", &(examples::Camera::getInstance().getCameraUp())[0] );
	shaderProgram->set3fv("CameraRight", &(examples::Camera::getInstance().getCameraRight())[0] );

	shaderProgram->use();
	VAO->bind();

	shaderProgram->getContext().glDrawArrays(GL_POINTS, 0, container->size());

	//std::cout << particleCount << " -- " << particleCenters->getSize() << "\n";
}
