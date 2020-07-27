#pragma once
#include <memory>
#include <string>
#include <geGL/Texture.h>

namespace ge
{
	namespace examples
	{
		class TextureHelper
		{
		public:
			static GLuint loadDDS(std::string imagePath, const gl::Context& context);
		};
	}
}
