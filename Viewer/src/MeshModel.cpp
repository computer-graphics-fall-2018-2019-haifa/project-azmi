#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	faces(faces),
	vertices(vertices),
	normals(normals),
	showNormals(false),
	showBoundingBox(false),
	worldTransform(glm::mat4x4(1))
{
}

MeshModel::~MeshModel()
{

}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

void MeshModel::applyTransformation(glm::mat4x4 & T)
{
	for (int v = 0; v < vertices.size(); v++) {
		glm::vec4 vert = glm::vec4(vertices.at(v), 1);
		glm::vec4 res =  vert * T;
		vertices.at(v).x = res.x;
		vertices.at(v).y = res.y;
		vertices.at(v).z = res.z;
	}

	for (int v = 0; v < BoundingBox.size(); v++) {
		glm::vec4 BBp = glm::vec4(BoundingBox.at(v), 1);
		glm::vec4 res = BBp * T;
		BoundingBox.at(v).x = res.x;
		BoundingBox.at(v).y = res.y;
		BoundingBox.at(v).z = res.z;
	}
}

void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

/*
void MeshModel::WindowResizeHandling(int WindowsizeX,int WindowsizeY)
{
	// vertices normalization
	int windowSize = fmin(WindowsizeX, WindowsizeY);
	float xmax = vertices.at(0).x; float xmin = vertices.at(0).x;
	float ymax = vertices.at(0).y; float ymin = vertices.at(0).y;
	float zmax = vertices.at(0).z; float zmin = vertices.at(0).z;
	
	for (int v = 0; v < vertices.size(); v++) {
		if (vertices.at(v).x > xmax)
			xmax = vertices.at(v).x;
		if (vertices.at(v).x < xmin)
			xmin = vertices.at(v).x;
		if (vertices.at(v).y > ymax)
			ymax = vertices.at(v).y;
		if (vertices.at(v).y < ymin)
			ymin = vertices.at(v).y;
		if (vertices.at(v).z > zmax)
			zmax = vertices.at(v).z;
		if (vertices.at(v).z < zmin)
			zmin = vertices.at(v).z;
	}


	for (int v = 0; v < vertices.size(); v++) {
		// scale vertices to [0,windowSize]
		vertices.at(v).x = windowSize*(vertices.at(v).x - xmin);
		vertices.at(v).y = windowSize*(vertices.at(v).y - ymin);
		vertices.at(v).z = windowSize*(vertices.at(v).z - zmin);

	}

	// normals normalization

	float xmaxn = normals.at(0).x; float xminn = normals.at(0).x;
	float ymaxn = normals.at(0).y; float yminn = normals.at(0).y;
	float zmaxn = normals.at(0).z; float zminn = normals.at(0).z;
	for (int v = 0; v < normals.size(); v++) {
		if (normals.at(v).x > xmaxn)
			xmaxn = normals.at(v).x;
		if (normals.at(v).x < xminn)
			xminn = normals.at(v).x;
		if (normals.at(v).y > ymaxn)
			ymaxn = normals.at(v).y;
		if (normals.at(v).y < yminn)
			yminn = normals.at(v).y;
		if (normals.at(v).z > zmaxn)
			zmaxn = normals.at(v).z;
		if (normals.at(v).z < zminn)
			zminn = normals.at(v).z;
	}


	for (int n = 0; n < normals.size(); n++) {
		// scale Normals to [-0.01 * windowSize , 0.01* windowSize]
		normals.at(n).x = 0.1*windowSize*(2*(normals.at(n).x - xminn) / (xmaxn - xminn) - 1);
		normals.at(n).y = 0.1*windowSize*(2*(normals.at(n).y - yminn) / (ymaxn - yminn) - 1);
		normals.at(n).z = 0.1*windowSize*(2*(normals.at(n).z - zminn) / (zmaxn - zminn) - 1);
	}

}

*/

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}

bool MeshModel::getShowNormals()
{
	return showNormals;
}

void MeshModel::setShowNormals(bool set)
{
	this->showNormals = set;
}

bool MeshModel::getShowBoundingBox()
{
	return showBoundingBox;
}

void MeshModel::setShowBoundingBox(bool set)
{
	this->showBoundingBox = set;
}

std::vector<glm::vec3> MeshModel::getEdgePoints()
{
	std::vector<glm::vec3> vertices = this->getVertices();
	float xmax = vertices.at(0).x; float xmin = vertices.at(0).x;
	float ymax = vertices.at(0).y; float ymin = vertices.at(0).y;
	float zmax = vertices.at(0).z; float zmin = vertices.at(0).z;

	for (int v = 0; v < vertices.size(); v++) {
		if (vertices.at(v).x > xmax)
			xmax = vertices.at(v).x;
		if (vertices.at(v).x < xmin)
			xmin = vertices.at(v).x;
		if (vertices.at(v).y > ymax)
			ymax = vertices.at(v).y;
		if (vertices.at(v).y < ymin)
			ymin = vertices.at(v).y;
		if (vertices.at(v).z > zmax)
			zmax = vertices.at(v).z;
		if (vertices.at(v).z < zmin)
			zmin = vertices.at(v).z;
	}

	std::vector<glm::vec3> edges;
	edges.push_back(glm::vec3(xmin, ymin, zmin));
	edges.push_back(glm::vec3(xmax, ymax, zmax));
	
	return edges;
}

glm::vec3 MeshModel::getPinningPoint()
{
	std::vector<glm::vec3> vertices = this->getVertices();
	float x=0, y=0, z=0;
	float s = vertices.size();
	for (int v = 0; v < s; v++) {
		// scale vertices to [0,windowSize]
		x += vertices.at(v).x;
		y += vertices.at(v).y;
		z += vertices.at(v).z;
	}
	return glm::vec3(x/s,y/s,z/s);
}

std::vector<Face> MeshModel::getFaces()
{
	return faces;
}

std::vector<glm::vec3> MeshModel::getNormals()
{
	return normals;
}

std::vector<glm::vec3> MeshModel::getBoundingBox()
{
	this->BoundingBox.clear();
	float xmax = vertices.at(0).x; float xmin = vertices.at(0).x;
	float ymax = vertices.at(0).y; float ymin = vertices.at(0).y;
	float zmax = vertices.at(0).z; float zmin = vertices.at(0).z;
	for (int v = 0; v < vertices.size(); v++) {
		if (vertices.at(v).x > xmax)
			xmax = vertices.at(v).x;
		if (vertices.at(v).x < xmin)
			xmin = vertices.at(v).x;
		if (vertices.at(v).y > ymax)
			ymax = vertices.at(v).y;
		if (vertices.at(v).y < ymin)
			ymin = vertices.at(v).y;
		if (vertices.at(v).z > zmax)
			zmax = vertices.at(v).z;
		if (vertices.at(v).z < zmin)
			zmin = vertices.at(v).z;
	}
	
	// upper box face
	this->BoundingBox.push_back(glm::vec3(xmin, ymax, zmax));
	this->BoundingBox.push_back(glm::vec3(xmin, ymin, zmax));
	this->BoundingBox.push_back(glm::vec3(xmax, ymin, zmax));
	this->BoundingBox.push_back(glm::vec3(xmax, ymax, zmax));
	// bottom
	this->BoundingBox.push_back(glm::vec3(xmin, ymax, zmin));
	this->BoundingBox.push_back(glm::vec3(xmin, ymin, zmin));
	this->BoundingBox.push_back(glm::vec3(xmax, ymin, zmin));
	this->BoundingBox.push_back(glm::vec3(xmax, ymax, zmin));
	
	return this->BoundingBox;
}

std::vector<glm::vec3> MeshModel::getVertices()
{
	return vertices;
}

