#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace ge {
	namespace examples {
		class Camera {
		public:
			static Camera &getInstance() {
				static Camera instance;
				return instance;
			}
			Camera(Camera const&) = delete;
			void operator=(Camera const&) = delete;

			glm::mat4 getMVP();
			glm::vec3 getCameraUp();
			glm::vec3 getCameraRight();

			void calculateMVP();
			void calculateCameraVectors();

			void setWindowDimension(float width, float height);
			void setCameraPos(glm::vec3 pos);

		private:
			Camera();

			glm::mat4 mvp;
			glm::mat4 projection;
			glm::mat4 view;
			glm::vec3 cameraUp;
			glm::vec3 cameraRight;
		};
	}
}


inline glm::mat4 ge::examples::Camera::getMVP()
{
	return mvp;
}

inline glm::vec3 ge::examples::Camera::getCameraUp()
{
	return cameraUp;
}

inline glm::vec3 ge::examples::Camera::getCameraRight()
{
	return cameraRight;
}

inline void ge::examples::Camera::calculateMVP()
{
	glm::mat4 Model = glm::mat4(1.0f);

	calculateCameraVectors();

	mvp = projection * view * Model;
}

inline void ge::examples::Camera::calculateCameraVectors()
{
	cameraRight = glm::vec3(view[0][0], view[1][0], view[2][0]);
	cameraUp = glm::vec3(view[0][1], view[1][1], view[2][1]);
}

inline void ge::examples::Camera::setWindowDimension(float width, float height)
{
	projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

	calculateMVP();
}

inline void ge::examples::Camera::setCameraPos(glm::vec3 pos)
{
	view = glm::lookAt(
		//glm::vec3(0, 0, -5), // Camera is at (4,3,3), in World Space
		pos,
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	calculateMVP();
}


