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

bool showDemoWindow = true;
bool showAnotherWindow = false;
bool showUtilityWindow = true;
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
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
	
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if(showUtilityWindow){
		static float f = 0.0f;
		static int counter = 0;

		// title --------------------------------------------------------------------------------------------
		ImGui::Begin("Utilities");                          

		// background color ---------------------------------------------------------------------------------
		ImGui::ColorEdit3("Background color", (float*)&clearColor); 

		// show normals and bounding box --------------------------------------------------------------------
		ImGui::Checkbox("show normals", &showNormals);      
		ImGui::Checkbox("show bounding box", &showBoundingBox);
		
		if (showNormals) {
			// draw normals
			if (scene.GetActiveModelIndex() >= 0)
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


		// list all models and cameras and choose active -----------------------------------------------

		if (ImGui::TreeNode("All models"))
		{
			//ImGui::Text("Without border:");
			ImGui::Columns(1, "mycolumns1", false);  // 3-ways, no border
			ImGui::Separator();
			for (int n = 0; n < scene.GetModelCount(); n++)
			{
				char label[32];
				sprintf(label, "%s", scene.getModelsNames().at(n).c_str());
				if (ImGui::Selectable(label)) {
					scene.SetActiveModelIndex(n);
				}
				//ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("All cameras"))
		{
			//ImGui::Text("Without border:");
			ImGui::Columns(1, "mycolumns1", false);  // 3-ways, no border
			ImGui::Separator();
			for (int n = 0; n < scene.GetCameraCount(); n++)
			{
				char label[32];
				sprintf(label, "camera %d", &n);
				if (ImGui::Selectable(label)) {
					scene.SetActiveCameraIndex(n);
				}
				//ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}


		int ActiveModelIndex = scene.GetActiveModelIndex();
		std::string name;
		if (ActiveModelIndex < 0)
			name = "";
		else
			name = scene.getModelsNames().at(scene.GetActiveModelIndex());
		ImGui::Text("Active model is %s", name.c_str());
		
		int ActiveCameraIndex = scene.GetActiveCameraIndex();
		std::string CameraName;
		if (ActiveCameraIndex < 0)
			CameraName = "";
		else
			CameraName =  std::to_string(ActiveCameraIndex);
		ImGui::Text("Active camera is %s", CameraName.c_str());
		
		


		// operations and transformations -------------------------------------------------------------------

		// translation
		ImGui::Text("Translation");
		static float step=0.0f;
		ImGui::SliderFloat("", &step, -100.0f, 100.0f, "translation step size = %.3f");
		if (ImGui::Button("translate X")) {                          // Buttons return true when clicked (most widgets return true when edited/activated)
			scene.translateModel(scene.GetActiveModelIndex(),step, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button("translate Y")) {
			scene.translateModel(scene.GetActiveModelIndex(),step, 1);
		}
		
		// rotation
		ImGui::Text("Rotation");
		static float angle = 0.0f;
		ImGui::SliderAngle("slider angle", &angle);
		if (ImGui::Button("rotate X")) {                          // Buttons return true when clicked (most widgets return true when edited/activated)
			scene.rotateModel(scene.GetActiveModelIndex(),angle, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button("rotate Y")) {                          // Buttons return true when clicked (most widgets return true when edited/activated)
			scene.rotateModel(scene.GetActiveModelIndex(),angle, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button("rotate Z")) {                          // Buttons return true when clicked (most widgets return true when edited/activated)
			scene.rotateModel(scene.GetActiveModelIndex(),angle, 2);
		}

		// scaling
		ImGui::Text("Scaling");
		static float factor = 1.0f;
		ImGui::SliderFloat("scale", &factor, 0.0f, 2.0f, "scale factor = %.3f");
		if (ImGui::Button("scale X")) {                          // Buttons return true when clicked (most widgets return true when edited/activated)
			scene.scaleModel(scene.GetActiveModelIndex(),factor, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button("scale Y")) {
			scene.scaleModel(scene.GetActiveModelIndex(),factor, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button("scale Z")) {
			scene.scaleModel(scene.GetActiveModelIndex(),factor, 2);
		}

		ImGui::End();
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