#include "ExampleManager.h"

#include <imgui.h>
#include <iostream>

#include "SimpleExample.h"
#include <geParticle/ParticleSystemManager.h>

ge::examples::ExampleManager::ExampleManager()
{
	manager = std::make_shared<particle::ParticleSystemManager>();
	
	examples.push_back(std::make_shared<SimpleExample>(manager));
}

void ge::examples::ExampleManager::render()
{
	renderGui();

	manager->update(core::time_point::clock::now());

	if (activeExampleIdx >= 0)
		examples[activeExampleIdx]->render();
}

void ge::examples::ExampleManager::renderGui()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::Begin("Example Browser");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	std::vector<std::string> names;
	for(auto &example : examples)
	{
		names.push_back(example->getName());
	}

	static int listbox_item_current = -1;
	if (ImGui::ListBox(
		"Choose example",
		&listbox_item_current,
		[](void* data, int n, const char** out_text) -> bool
	{
		auto & v = *static_cast<std::vector<std::string>*>(data);
		*out_text = v.at(n).c_str();
		return true;
	},
		static_cast<void*>(&names),
		examples.size(),
		4
		))
	{
		exampleChanged(listbox_item_current);
	}

	if (ImGui::Button("Play"))
		manager->startAll(core::time_point::clock::now());
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
		manager->pauseAll();
	
	ImGui::End();

	if (activeExampleIdx >= 0)
		examples[activeExampleIdx]->renderGui();
}

void ge::examples::ExampleManager::exampleChanged(int newIdx)
{
	if (activeExampleIdx >= 0)
		examples[activeExampleIdx]->reset();
	
	examples[newIdx]->init();
	manager->startAll(core::time_point::clock::now());
	
	activeExampleIdx = newIdx;
}
