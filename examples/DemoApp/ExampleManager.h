/** @file ExampleManager.h
 *  @brief Covers examples. Render gui and do example selection.
 *  @author Jan Sobol xsobol04
 */

#pragma once
#include <imgui.h>
#include <string>
#include <vector>
#include <geCore/Updatable.h>
#include <glm/glm.hpp>
#include <memory>

namespace ge
{
	namespace particle
	{
		class ParticleSystemManager;
	}
	
	namespace examples
	{
		/**
		 * @brief An example abstract class.
		 */
		class Example
		{
		public:
			virtual ~Example() = default;
			virtual void init() = 0;
			virtual void render() = 0;
			virtual void reset() = 0;

			virtual void renderGui() = 0;
			
			virtual std::string getName() const = 0;

			virtual unsigned int getContainerSize() const = 0;

			virtual void stopEmitting() {};

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

		/**
		 * @brief Covers examples. Render gui and do example selection.
		 */
		class ExampleManager
		{
		public:
			ExampleManager();
			void render();

			void toggleShowGui() { showGui = !showGui; }
			void togglePaused();
			void toggleRealTime();
			void stopEmitting();

		private:
			void renderGui();
			void exampleChanged(int newIdx);

			core::time_point time() const;
			
			std::vector<std::shared_ptr<Example>> examples;
			int activeExampleIdx = -1;

			std::shared_ptr<particle::ParticleSystemManager> manager;

			bool showGui = true;
			bool paused = false;

			bool realTime = true;
			core::time_unit frameTime = core::time_unit(0.016f);
			core::time_point simTime = core::time_point::clock::now();
		};
	}
}
