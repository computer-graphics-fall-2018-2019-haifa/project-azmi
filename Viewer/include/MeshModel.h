#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */
class MeshModel
{
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> BoundingBox;
	std::vector<glm::vec3> normals;
	glm::mat4x4 worldTransform;
	glm::vec4 color;
	std::string modelName;
	glm::vec3 pinningPoint;
	bool showNormals;
	bool showBoundingBox;

public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;
	void applyTransformation(glm::mat4x4& T);
	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);
	//void WindowResizeHandling(int WindowsizeX , int WindowsizeY);
	const std::string& GetModelName();
	bool getShowNormals();
	void setShowNormals(bool set);
	bool getShowBoundingBox();
	void setShowBoundingBox(bool set);
	std::vector<glm::vec3> getEdgePoints();
	glm::vec3 getPinningPoint();
	std::vector<Face> getFaces();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getBoundingBox();
	std::vector<glm::vec3> getVertices();
};
