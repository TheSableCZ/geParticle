#include "SimpleExampleRenderer.h"

#include <geGL/Program.h>
#include <geGL/Shader.h>
#include <geUtil/Text.h>
#include <geParticleStd/StandardParticleComponents.h>

#include "CameraSingleton.h"
#include "SimpleExample.h"

ge::examples::SimpleExampleRenderer::SimpleExampleRenderer(std::shared_ptr<particle::GPUParticleContainer> container)
{
	std::string vexShd = ge::util::loadTextFile(APP_RESOURCES"/Simple/vertexShader.glsl");
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, vexShd);
	std::string frgShd = ge::util::loadTextFile(APP_RESOURCES"/Simple/fragmentShader.glsl");
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, frgShd);
	std::string gShd = ge::util::loadTextFile(APP_RESOURCES"/Simple/billboardGeometryShader.glsl");
	std::shared_ptr<ge::gl::Shader> geometryShader = std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, gShd);
	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader, geometryShader);


	//billboardVertices = std::make_shared<ge::gl::Buffer>(particleVertices.size() * sizeof(float), particleVertices.data());

	/*
	particleCenters = std::make_shared<ge::gl::Buffer>(maxParticles * sizeof(float) * 3);
	centers = std::vector<float>();
	*/

	/*
	particleColors = std::make_shared<ge::gl::Buffer>(maxParticles * sizeof(float) * 3);
	*/

	VAO = std::make_shared<ge::gl::VertexArray>();
	//VAO->addAttrib(billboardVertices, 0, 3, GL_FLOAT);
	//VAO->addAttrib(particleCenters, 1, 3, GL_FLOAT);
	//VAO->addAttrib(particleColors, 2, 3, GL_FLOAT);

	container->addComponentVertexAttrib<particle::Position>(VAO, 0, 3, GL_FLOAT, sizeof(particle::Position), offsetof(particle::Position, position));
	container->addComponentVertexAttrib<particle::Color>(VAO, 1, 3, GL_FLOAT, sizeof(particle::Color), offsetof(particle::Color, color));
}

void ge::examples::SimpleExampleRenderer::render(std::shared_ptr<particle::ParticleContainer> container)
{
	auto gpuContainer = std::static_pointer_cast<particle::GPUParticleContainer>(container);

	auto particlesCount = gpuContainer->syncOnlyAlive(particle::GPUParticleContainer::CPU_TO_GPU);

	shaderProgram->setMatrix4fv("viewMatrix", &(CameraSingleton::getInstance().getViewMatrix())[0][0]);
	shaderProgram->setMatrix4fv("projectionMatrix", &(CameraSingleton::getInstance().getProjectionMatrix())[0][0]);
	shaderProgram->set3fv("CameraUp", &(CameraSingleton::getInstance().getCameraUp())[0]);
	shaderProgram->set3fv("CameraRight", &(CameraSingleton::getInstance().getCameraRight())[0]);

	shaderProgram->use();
	VAO->bind();

	shaderProgram->getContext().glDrawArrays(GL_POINTS, 0, particlesCount);

	/*gl->glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	gl->glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	gl->glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	gl->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);*/
}
