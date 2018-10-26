#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3* viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, color);
		}
	}
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}


void Renderer::Render(Scene& scene)
{

	// draw the active model
	if (scene.GetModelCount() > 0) {
		std::shared_ptr<MeshModel> ActiveModel = scene.getActiveModel();
		plotActiveModel(ActiveModel);
	}
	
	
}


void Renderer::plotActiveModel(std::shared_ptr<MeshModel> ActiveModel) {
	int shithappens = 0;
	MeshModel model = *(ActiveModel.get());
	plotFaces(model.getFaces(), normalizeVertices(model.getVertices()));
}



void Renderer::plotFaces(std::vector<Face> Faces, std::vector<glm::vec3> Vertices)
{
	for (int faceNum = 0; faceNum < Faces.size(); faceNum++) {
		Face f = Faces.at(faceNum);
		std::vector<glm::vec3> faceVertices;
		for (int v = 0; v < 3; v++) {
			faceVertices.push_back(Vertices.at(f.GetVertexIndex(v)-1));
		}
		plotFaceLines(faceVertices);
	}
}

void Renderer::plotFaceLines(std::vector<glm::vec3> Vertices)
{
	for (int v = 0; v < 3; v++) {
		glm::vec3 p1 = Vertices.at(v);
		glm::vec3 p2 = Vertices.at((v + 1)%3);
		plotLine(p1, p2);
	}
}

std::vector<glm::vec3> Renderer::normalizeVertices(std::vector<glm::vec3> Vertices)
{
	float xmax = Vertices.at(0).x; float xmin = Vertices.at(0).x;
	float ymax = Vertices.at(0).y; float ymin = Vertices.at(0).y;
	for (int v=0; v < Vertices.size(); v++) {
		if (Vertices.at(v).x > xmax)
			xmax = Vertices.at(v).x;
		if (Vertices.at(v).x < xmin)
			xmin = Vertices.at(v).x;
		if (Vertices.at(v).y > ymax)
			ymax = Vertices.at(v).y;
		if (Vertices.at(v).y < ymin)
			ymin = Vertices.at(v).y;
	}

	for (int v=0; v < Vertices.size(); v++) {
		Vertices.at(v).x = (viewportWidth / 4) + (viewportWidth/2)*((Vertices.at(v).x - xmin) / (xmax - xmin));
		Vertices.at(v).y = (viewportHeight / 4) + (viewportHeight/2)*((Vertices.at(v).y - ymin) / (ymax - ymin));
	}

	return Vertices;
}

void Renderer::plotLine(glm::vec3 point1, glm::vec3 point2)
{
	// drawing accourding to the Bresenham algorithm
	
	// if x1==x2
	if (round(point1.x) == round(point2.x)) {
		int y1 = fmin(point1.y, point2.y);
		int y2 = fmax(point1.y, point2.y);
		for (int y = y1; y <= y2; y++)
			putPixel(round(point1.x),round(y), glm::vec3(0, 0, 0));
		return;
	}

	// if y1==y2
	if (round(point1.y) == round(point2.y)) {
		int x1 = fmin(point1.x, point2.x);
		int x2 = fmax(point1.x, point2.x);
		for (int x = x1; x <= x2; x++)
			putPixel(round(x), round(point1.y), glm::vec3(0, 0, 0));
		return;
	}
	
	float dy = (point2.y - point1.y); float dx = (point2.x - point1.x);
	if (abs(dy) < abs(dx)) {
		if (point2.x < point1.x)
			plotLineLow(point2.x, point2.y, point1.x, point1.y);
		else
			plotLineLow(point1.x, point1.y, point2.x, point2.y);
	}
	else{
		if (point2.y < point1.y)
			plotLineHigh(point2.x, point2.y, point1.x, point1.y);
		else
			plotLineHigh(point1.x, point1.y, point2.x, point2.y);
	}
}

void Renderer::plotLineLow(float x0, float y0, float x1, float y1) {
	float dx = x1 - x0;
	float dy = y1 - y0;
	float yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	float D = 2 * dy - dx;
	float y = y0;
		
	for (int x = x0; x <= x1; x++) {
		putPixel(round(x), round(y), glm::vec3(0, 0, 0));
		if (D > 0){
			y = y + yi;
			D = D - 2 * dx;
		}
		D = D + 2 * dy;
	}
}

void Renderer::plotLineHigh(float x0, float y0, float x1, float y1) {
	float dx = x1 - x0;
	float dy = y1 - y0;
	float xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}			
	float D = 2 * dx - dy;
	float x = x0;

	for (int y = y0; y <= y1; y++) {
		putPixel(round(x), round(y), glm::vec3(0, 0, 0));
		if (D > 0) {
			x = x + xi;
			D = D - 2 * dy;
		}
		D = D + 2 * dx;
	}
}














//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}