#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Camera> cameras;

	int windowSizeW;
	int windowSizeH;

	int activeCameraIndex;
	int activeModelIndex;

public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;
	std::vector<std::string> getModelsNames();
	void AddCamera(const Camera& camera);
	const int GetCameraCount() const;

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	// Add more methods as needed...
	void translateModel(int modelIndex,float step,int axes);
	void scaleModel(int modelIndex,float factor, int axes);
	void rotateModel(int modelIndex, float angle,int axes);
	void checkWindowSizeChangeAndHandle(int WindowSizeX, int WindowSizeY);
	void WindowResizeHandling(int WindowSizeX, int WindowSizeY);
	void SetWindowSize(int w, int h);
	std::shared_ptr<MeshModel> getActiveModel();
	std::vector<std::shared_ptr<MeshModel>> getAllModels();
};