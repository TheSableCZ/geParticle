/** @file SimpleExampleRenderer.cpp
 *  @brief Simple particle renderer. Used by SimpleExample and EmittersTestExample.
 *  @author Jan Sobol xsobol04
 */

#include "SimpleExampleRenderer.h"

#include <geGL/Program.h>
#include <geGL/Shader.h>
#include <geUtil/Text.h>
#include <geParticleStd/StandardParticleComponents.h>

#include "CameraSingleton.h"

ge::examples::SimpleExampleRenderer::SimpleExampleRenderer(std::shared_ptr<particle::GPUParticleContainer> container)
{
	const std::string vexShd = 
#include "Simple/vertexShader.glsl"
	;
	//std::string vexShd = ge::util::loadTextFile(APP_RESOURCES"/Simple/vertexShader.glsl");
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, vexShd);

	const std::string frgShd =
#include "Simple/fragmentShader.glsl"
		;
	//std::string frgShd = ge::util::loadTextFile(APP_RESOURCES"/Simple/fragmentShader.glsl");
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, frgShd);

	const std::string gShd =
#include "Simple/billboardGeometryShader.glsl"
		;
	//std::string gShd = ge::util::loadTextFile(APP_RESOURCES"/Simple/billboardGeometryShader.glsl");
	std::shared_ptr<ge::gl::Shader> geometryShader = std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, gShd);
	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader, geometryShader);

	VAO = std::make_shared<ge::gl::VertexArray>();

	container->addComponentVertexAttrib<particle::Position>(VAO, 0, 3, GL_FLOAT, sizeof(particle::Position), offsetof(particle::Position, position));
	container->addComponentVertexAttrib<particle::Color>(VAO, 1, 4, GL_FLOAT, sizeof(particle::Color), offsetof(particle::Color, color));
}

void ge::examples::SimpleExampleRenderer::render(std::shared_ptr<particle::ParticleContainer> container)
{
	shaderProgram->getContext().glEnable(GL_BLEND);
	shaderProgram->getContext().glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	auto gpuContainer = std::static_pointer_cast<particle::GPUParticleContainer>(container);

	auto particlesCount = gpuContainer->syncOnlyAlive(particle::GPUParticleContainer::CPU_TO_GPU);

	shaderProgram->setMatrix4fv("viewMatrix", &(CameraSingleton::getInstance().getViewMatrix())[0][0]);
	shaderProgram->setMatrix4fv("projectionMatrix", &(CameraSingleton::getInstance().getProjectionMatrix())[0][0]);
	shaderProgram->set3fv("CameraUp", &(CameraSingleton::getInstance().getCameraUp())[0]);
	shaderProgram->set3fv("CameraRight", &(CameraSingleton::getInstance().getCameraRight())[0]);

	shaderProgram->use();
	VAO->bind();

	shaderProgram->getContext().glDrawArrays(GL_POINTS, 0, particlesCount);
}
