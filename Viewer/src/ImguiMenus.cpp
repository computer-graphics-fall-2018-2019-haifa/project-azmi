#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>

bool showDemoWindow = false;
bool showAnotherWindow = false;
bool showSimplewindow2 = true;
bool showLoadModelWindow = true;
bool showNormals = false;
bool showBoundingBox = false;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/*
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
	*/
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if(showSimplewindow2){
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Utilities");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Checkbox("show normals", &showNormals);      // Edit bools storing our window open/close state
		ImGui::Checkbox("show bounding box", &showBoundingBox);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("translate left"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			// code for translation right
		if (ImGui::Button("translate right"))

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if (showNormals){
		// draw normals
		if(scene.GetActiveModelIndex()>=0)
			(*scene.getActiveModel()).setShowNormals(true);
	}
	else {
		if (scene.GetActiveModelIndex() >= 0)
			(*scene.getActiveModel()).setShowNormals(false);
	}
	
	if (showBoundingBox) {
		// draw bounding box
		if (scene.GetActiveModelIndex() >= 0)
			(*scene.getActiveModel()).setShowBoundingBox(true);
	}
	else {
		if (scene.GetActiveModelIndex() >= 0)
			(*scene.getActiveModel()).setShowBoundingBox(false);
	}

	/*
	// 3. Show another simple window.
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
		{
			showAnotherWindow = false;
		}
		ImGui::End();
	}
	*/
	// 4. Demonstrate creating a fullscreen menu bar and populating it.
	if(showLoadModelWindow){
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File", showLoadModelWindow))
			{
				if (ImGui::MenuItem("Load Model...", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						MeshModel model = Utils::LoadMeshModel(outPath);
						scene.AddModel(std::make_shared<MeshModel>(model));
						//scene.AddCamera(Camera());
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}