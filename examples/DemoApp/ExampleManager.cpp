#include "ExampleManager.h"

#include <imgui.h>
#include <iostream>

#include "examples/SimpleExample/SimpleExample.h"
#include "examples/EmittersTestExample/EmittersTestExample.h"
#include <geParticle/ParticleSystemManager.h>

#include "examples/FireworkExample/FireworkExample.h"
#include "examples/GPUNbodyExample/GPUNbodyExample.h"
#include "examples/GPURainExample/GPURainExample.h"

ge::examples::ExampleManager::ExampleManager()
{
	manager = std::make_shared<particle::ParticleSystemManager>();
	
	examples.push_back(std::make_shared<SimpleExample>(manager));
	examples.push_back(std::make_shared<EmittersTestExample>(manager));
	examples.push_back(std::make_shared<FireworkExample>(manager));
	examples.push_back(std::make_shared<GPURainExample>(manager));
	examples.push_back(std::make_shared<GPUNbodyExample>(manager));
}

void ge::examples::ExampleManager::render()
{
	if (!realTime)
	{
		simTime += frameTime;
	}
	
	if (showGui)
		renderGui();

	manager->update(time());

	if (activeExampleIdx >= 0)
		examples[activeExampleIdx]->render();
}

void ge::examples::ExampleManager::togglePaused()
{	
	paused = !paused;
	if (paused) 
		manager->pauseAll();
	else
		manager->startAll(time());
}

void ge::examples::ExampleManager::toggleRealTime()
{
	realTime = !realTime;
	simTime = core::time_point::clock::now();
	manager->startAll(time());
}

void ge::examples::ExampleManager::renderGui()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::Begin("Example Browser");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	ImGui::Text("Particle container size: %d", activeExampleIdx >= 0 ? examples[activeExampleIdx]->getContainerSize() : 0);

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

	if (ImGui::Button("Play")) {
		paused = false;
		manager->startAll(time());
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause")) {
		paused = true;
		manager->pauseAll();
	}

	bool isRealTime = realTime;
	if (ImGui::Checkbox("realtime", &isRealTime))
	{
		toggleRealTime();
	}
	int frtm = static_cast<int>(frameTime.count() * 1000);
	if (ImGui::SliderInt("frame time (ms)", &frtm, 1, 100))
	{
		frameTime = core::time_unit(frtm / 1000.f);
	}

	ImGui::Text("Use keyboard arrows and mouse scroll to change camera view");
	
	ImGui::End();

	if (activeExampleIdx >= 0)
		examples[activeExampleIdx]->renderGui();
}

void ge::examples::ExampleManager::exampleChanged(int newIdx)
{
	if (activeExampleIdx >= 0)
		examples[activeExampleIdx]->reset();
	
	examples[newIdx]->init();

	if (!paused) {
		manager->startAll(time());
	}
	
	activeExampleIdx = newIdx;
}

ge::core::time_point ge::examples::ExampleManager::time() const
{
	if (realTime)
	{
		return core::time_point::clock::now();
	}
	else
	{
		return simTime;
	}
}
