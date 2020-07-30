/** @file FireworkRenderer.cpp
 *  @brief Renderer for firework effect.
 *  @author Jan Sobol xsobol04
 */

#include "FireworkRenderer.h"

#include <geGL/Program.h>

#include "CameraSingleton.h"
#include "FireworkExample.h"
#include "examples/Common/TextureHelper.h"
#include "geParticleStd/StandardParticleComponents.h"

ge::examples::FireworkRenderer::FireworkRenderer(std::shared_ptr<particle::GPUParticleContainer> container)
{
	const std::string vexShd =
#include "Firework/fireworkVertexShader.glsl"
		;
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, vexShd);

	const std::string frgShd =
#include "texturedBillboardFS.glsl"
		;
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, frgShd);

	const std::string gShd =
#include "Simple/billboardGeometryShader.glsl"
		;
	std::shared_ptr<ge::gl::Shader> geometryShader = std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, gShd);
	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader, geometryShader);


	VAO = std::make_shared<ge::gl::VertexArray>();

	container->addComponentVertexAttrib<particle::Position>(VAO, 0, 3, GL_FLOAT, sizeof(particle::Position), offsetof(particle::Position, position));
	container->addComponentVertexAttrib<particle::Color>(VAO, 1, 4, GL_FLOAT, sizeof(particle::Color), offsetof(particle::Color, color));
	container->addComponentVertexAttrib<Size>(VAO, 2, 2, GL_FLOAT, sizeof(Size), offsetof(Size, size));

#if defined(INDIR_RESOURCES)
	const std::string texturePath = "smoke.DDS";
#else
	const std::string texturePath = APP_RESOURCES"/textures/smoke.dds";
#endif
	texture = TextureHelper::loadDDS(texturePath, shaderProgram->getContext());
	textureUniformID = shaderProgram->getUniformLocation("myTextureSampler");
}

void ge::examples::FireworkRenderer::render(std::shared_ptr<particle::ParticleContainer> container)
{
	shaderProgram->getContext().glClearColor(0.04f, 0.04f, 0.173f, 1.f);
	shaderProgram->getContext().glClear(GL_COLOR_BUFFER_BIT);
	
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

	shaderProgram->getContext().glActiveTexture(GL_TEXTURE0);
	shaderProgram->getContext().glBindTexture(GL_TEXTURE_2D, texture);
	shaderProgram->getContext().glUniform1i(textureUniformID, 0);

	shaderProgram->getContext().glDrawArrays(GL_POINTS, 0, particlesCount);
}
