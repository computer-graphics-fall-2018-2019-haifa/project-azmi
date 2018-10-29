#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(-1),
	activeModelIndex(-1)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
	SetActiveModelIndex(activeModelIndex);
	activeModelIndex++;
}

const int Scene::GetModelCount() const
{
	return models.size();
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
	SetActiveCameraIndex(activeCameraIndex);
	activeCameraIndex++;
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}




}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::SetActiveModelIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < models.size())
	{
		activeModelIndex = index;
	}

	for (int model = 0; model < models.size(); model++) {
		if (model != index) {
			models.at(model)->setShowBoundingBox(false);
			models.at(model)->setShowNormals(false);
		}
	}

}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}

void Scene::WindowResizeHandling(int WindowSizeX, int WindowSizeY)
{
	for (int model = 0; model < models.size(); model++)
		models.at(model)->WindowResizeHandling(WindowSizeX, WindowSizeY);
}

std::shared_ptr<MeshModel> Scene::getActiveModel()
{
	return models.at(GetActiveModelIndex());
}

std::vector<std::shared_ptr<MeshModel>> Scene::getAllModels()
{
	return models;
}
