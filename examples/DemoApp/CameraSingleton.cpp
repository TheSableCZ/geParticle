#include "CameraSingleton.h"

#include <geUtil/OrbitManipulator.h>

ge::examples::CameraSingleton::CameraSingleton()
	: BasicCamera()
{
	auto manip = std::make_shared<util::OrbitManipulator>();
	auto ptr = std::static_pointer_cast<util::BasicManipulatorInterface>(manip);
    //BasicCamera::setActiveManipulator(ptr);
    activeManipulator = ptr;
	BasicCamera::setMatrix(manip->getRefMatrix());

	projectionMatrix = std::make_shared<glm::mat4>();
}

glm::vec3 ge::examples::CameraSingleton::getCameraUp() const
{
	auto view = getViewMatrix();
	return glm::vec3(view[0][1], view[1][1], view[2][1]);
}

glm::vec3 ge::examples::CameraSingleton::getCameraRight() const
{
	auto view = getViewMatrix();
	return glm::vec3(view[0][0], view[1][0], view[2][0]);
}

void ge::examples::CameraSingleton::setWindowDimension(float width, float height) const
{
	*projectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
}

void ge::examples::CameraSingleton::setPosition(const glm::vec3& pos) const
{
	std::static_pointer_cast<util::OrbitManipulator>(activeManipulator)->setPosition(pos);
}

glm::mat4 ge::examples::CameraSingleton::getViewMatrix() const
{
	activeManipulator->getMatrix();
	return BasicCamera::getViewMatrix();
}
