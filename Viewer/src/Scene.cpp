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
	// WindowResizeHandling(this->windowSizeW, this->windowSizeH);
	SetActiveModelIndex(activeModelIndex);
	activeModelIndex++;
}

const int Scene::GetModelCount() const
{
	return models.size();
}

std::vector<std::string> Scene::getModelsNames()
{
	std::vector<std::string> Names;
	for (int model = 0; model < models.size(); model++) {
		Names.push_back(models.at(model)->GetModelName());
	}
	return Names;
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

void Scene::translateModel(int ModelIndex, float step, int axes)
{
	if (ModelIndex< 0)
		return;

	glm::mat4x4 translationMatrix = glm::mat4x4(1);
	translationMatrix[axes].w = step;
	this->getAllModels().at(ModelIndex)->applyTransformation(translationMatrix);
}

void Scene::scaleModel(int modelIndex , float factor, int axes)
{
	if (modelIndex < 0)
		return;
	glm::mat4x4 transformationMatrix = glm::mat4x4(1);
	if(axes==0)
		transformationMatrix[0].x = factor;
	else if (axes == 1)
		transformationMatrix[1].y = factor;
	else {
		transformationMatrix[2].z = factor;
	}
	this->getAllModels().at(modelIndex)->applyTransformation(transformationMatrix);

}

void Scene::rotateModel(int modelIndex,float angle, int axes)
{
	if (modelIndex < 0)
		return;
	glm::mat4x4 transformationMatrix = glm::mat4x4(1);

	if (axes == 0) {
		transformationMatrix[1].y = cos(angle); transformationMatrix[1].z = -sin(angle);
		transformationMatrix[2].y = sin(angle); transformationMatrix[2].z = cos(angle);

	}
	else if (axes == 1) {
		transformationMatrix[0].x = cos(angle); transformationMatrix[0].z = sin(angle);
		transformationMatrix[2].x = - sin(angle); transformationMatrix[2].z = cos(angle);
	}
	else {
		transformationMatrix[0].x = cos(angle); transformationMatrix[0].y = -sin(angle);
		transformationMatrix[1].x = sin(angle); transformationMatrix[1].y = cos(angle);

	}

	glm::vec3 p = this->getAllModels().at(modelIndex)->getPinningPoint();

	this->translateModel(modelIndex ,-p.x,0);
	this->translateModel(modelIndex ,-p.y, 1);
	this->translateModel(modelIndex ,-p.z, 2);
	this->getAllModels().at(modelIndex)->applyTransformation(transformationMatrix);
	this->translateModel(modelIndex,p.x, 0);
	this->translateModel(modelIndex,p.y, 1);
	this->translateModel(modelIndex,p.z, 2);

}

void Scene::checkWindowSizeChangeAndHandle(int NewWindowSizeW, int NewWindowSizeH)
{
	if (this->windowSizeW != NewWindowSizeW | this->windowSizeH != NewWindowSizeH) {
		this->WindowResizeHandling(NewWindowSizeW/ windowSizeW, NewWindowSizeH/ windowSizeH);
		this->SetWindowSize(NewWindowSizeW, NewWindowSizeH);
	}
}

void Scene::WindowResizeHandling(int scaleX, int scaleY)
{
	for (int model = 0; model < models.size(); model++) {
		glm::vec3 edgesmin = models.at(model)->getEdgePoints()[0];
		glm::vec3 edgesmax = models.at(model)->getEdgePoints()[1];
		
		float offsetX = 0, offsetY = 0;
		// translate
		if (edgesmin.x < 0) {
			this->translateModel(model, -edgesmin.x, 0);
			offsetX = -edgesmin.x;
		}
		if (edgesmin.y < 0) {
			this->translateModel(model, -edgesmin.y, 1);
			offsetY = -edgesmin.y;
		}
		// scale
		
		float scaleFactor = fmin(scaleX, scaleY);
		this->scaleModel(model, scaleFactor, 0);
		this->scaleModel(model, scaleFactor, 1);
		this->scaleModel(model, scaleFactor, 2);
	}
}

void Scene::SetWindowSize(int w, int h)
{
	this->windowSizeW = w;
	this->windowSizeH = h;
}

std::shared_ptr<MeshModel> Scene::getActiveModel()
{
	return models.at(GetActiveModelIndex());
}

std::vector<std::shared_ptr<MeshModel>> Scene::getAllModels()
{
	return models;
}
