#pragma once
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	void putPixel(int x, int y, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);

	// Add more methods/functionality as needed...
	void plotActiveModel(std::shared_ptr<MeshModel> ActiveModel);
	void plotFaces(std::vector<Face> Faces, std::vector<glm::vec3> Vertices);
	void plotFaceLines(std::vector<glm::vec3> Vertices);
	std::vector<glm::vec3> normalizeVertices(std::vector<glm::vec3> Vertices);
	void plotLine(glm::vec3 point1, glm::vec3 point2);
	void plotLineLow(float x0, float y0, float x1, float y1);
	void plotLineHigh(float x0, float y0, float x1, float y1);
};
