#pragma once
#include <imgui.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace ge
{
	namespace particle
	{
		class ParticleSystemManager;
	}
	
	namespace examples
	{
		class Example
		{
		public:
			virtual ~Example() = default;
			virtual void init() = 0;
			virtual void render() = 0;
			virtual void reset() = 0;

			virtual void renderGui() = 0;
			
			virtual std::string getName() const = 0;

			static void GuiRange(glm::vec2 &range, std::string label, float min, float max)
			{
				if (ImGui::TreeNode(label.c_str()))
				{
					ImGui::SliderFloat("min", &range.x, min, max);
					ImGui::SliderFloat("max", &range.y, min, max);
					ImGui::TreePop();
				}
			}

			static void GuiVec3(glm::vec3 &vector, std::string label, float min, float max)
			{
				if (ImGui::TreeNode(label.c_str()))
				{
					ImGui::SliderFloat("x", &vector.x, min, max);
					ImGui::SliderFloat("y", &vector.y, min, max);
					ImGui::SliderFloat("z", &vector.z, min, max);
					ImGui::TreePop();
				}
			}
		};
		
		class ExampleManager
		{
		public:
			ExampleManager();
			void render();

		private:
			void renderGui();
			void exampleChanged(int newIdx);
			
			std::vector<std::shared_ptr<Example>> examples;
			int activeExampleIdx = -1;

			std::shared_ptr<particle::ParticleSystemManager> manager;
		};
	}
}
