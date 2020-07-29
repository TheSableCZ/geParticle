#include "GPURainRenderer.h"

#include <geGL/Program.h>

#include "CameraSingleton.h"
#include "examples/Common/TextureHelper.h"
#include "geParticleStd/StandardParticleComponents.h"

ge::examples::GPURainRenderer::GPURainRenderer(std::shared_ptr<particle::GPUParticleContainer> container)
{
	const std::string vexShd =
#include "GPURain/vertexShader.glsl"
		;
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, vexShd);

	const std::string frgShd =
#include "texturedBillboardFS.glsl"
		;
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, frgShd);

	const std::string gShd =
#include "GPURain/billboardGeometryShader.glsl"
		;
	std::shared_ptr<ge::gl::Shader> geometryShader = std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, gShd);
	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader, geometryShader);


	VAO = std::make_shared<ge::gl::VertexArray>();

	container->addComponentVertexAttrib<particle::GPUMassPointData>(VAO, 0, 4, GL_FLOAT, sizeof(particle::GPUMassPointData), offsetof(particle::GPUMassPointData, position));
	container->addComponentVertexAttrib<particle::GPULifeData>(VAO, 1, 1, GL_FLOAT, sizeof(particle::GPULifeData), offsetof(particle::GPULifeData, life));

	texture = TextureHelper::loadDDS(APP_RESOURCES"/GPURain/particle.DDS", shaderProgram->getContext());
	textureUniformID = shaderProgram->getUniformLocation("myTextureSampler");
}

void ge::examples::GPURainRenderer::render(std::shared_ptr<particle::ParticleContainer> container)
{
	shaderProgram->getContext().glEnable(GL_BLEND);
	shaderProgram->getContext().glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	shaderProgram->setMatrix4fv("viewMatrix", &(CameraSingleton::getInstance().getViewMatrix())[0][0]);
	shaderProgram->setMatrix4fv("projectionMatrix", &(CameraSingleton::getInstance().getProjectionMatrix())[0][0]);
	shaderProgram->set3fv("CameraUp", &(CameraSingleton::getInstance().getCameraUp())[0]);
	shaderProgram->set3fv("CameraRight", &(CameraSingleton::getInstance().getCameraRight())[0]);

	shaderProgram->use();
	VAO->bind();

	shaderProgram->getContext().glActiveTexture(GL_TEXTURE0);
	shaderProgram->getContext().glBindTexture(GL_TEXTURE_2D, texture);
	shaderProgram->getContext().glUniform1i(textureUniformID, 0);

	shaderProgram->getContext().glDrawArrays(GL_POINTS, 0, container->size());
}
