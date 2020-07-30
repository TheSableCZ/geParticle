/** @file TextureHelper.h
 *  @brief DDS texture loader.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <memory>
#include <string>
#include <geGL/Texture.h>

namespace ge
{
	namespace examples
	{
		/**
		 * @brief DDS texture loader.
		 */
		class TextureHelper
		{
		public:
			static GLuint loadDDS(std::string imagePath, const gl::Context& context);
		};
	}
}
