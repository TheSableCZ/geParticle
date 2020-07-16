#pragma once

#include <geUtil/BasicCamera.h>

namespace ge
{
	namespace examples
	{
		class CameraSingleton : public util::BasicCamera
		{
		public:
			CameraSingleton();
			
			static CameraSingleton &getInstance()
			{
				static CameraSingleton instance;
				return  instance;
			}
			CameraSingleton(CameraSingleton const&) = delete;
			void operator=(CameraSingleton const&) = delete;

			glm::vec3 getCameraUp() const;
			glm::vec3 getCameraRight() const;

			void setWindowDimension(float width, float height) const;
			void setPosition(const glm::vec3& pos) const;

			glm::mat4 getViewMatrix() const override;
		};
	}
}
