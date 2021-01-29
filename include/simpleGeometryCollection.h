#ifndef SIMPLE_GEOMETRY_COLLECTION_H
#define SIMPLE_GEOMETRY_COLLECTION_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

using namespace std;

/***************************
 * 	SCREENSPACE QUAD
 * ************************/
class SimpleQuad
{
public:
	void upload();
	void draw();
protected:
	vector<unsigned int> indices;
	vector<glm::vec3> vertices;
	GLuint vbo[2];
	GLuint vao;
};

/***************************
 * 	LINE BOX
 * ************************/
class LineBox{
public:
	void upload();
	void draw();
protected:
	vector<unsigned int> indices;
	vector<glm::vec3> vertices;
	GLuint vbo[2];
	GLuint vao;
};

/***************************
 * 	COORDINATE SYSTEM
 * ************************/
class CoordinateSystem
{
public:
	void upload();
	void draw();

protected:

	vector<unsigned int> indices;
	vector<glm::vec3> vertices;

	GLuint vbo[2];
};

/***************************
 * 	GRID XZ
 * ************************/
class Grid_XZ
{
public:
	Grid_XZ(float distance, unsigned int numLines);
	void upload();
	void draw();
protected:
	float m_distance;
	float m_lineLength;
	unsigned int m_numLines;
	vector<unsigned int> indices;
	vector<glm::vec3> vertices;

	GLuint vbo[2];
	GLuint vao;
};

/***************************
 * 	GRID YZ
 * ************************/
class Grid_YZ
{
public:
	Grid_YZ(float distance, unsigned int numLines);
	void upload();
	void draw();
protected:
	float m_distance;
	float m_lineLength;
	unsigned int m_numLines;
	vector<unsigned int> indices;
	vector<glm::vec3> vertices;

	GLuint vbo[2];
	GLuint vao;
};

/***************************
 * 	SPHERE
 * ************************/
class SolidSphere
{
public:
	SolidSphere(const float radius, const int slices, const int stacks);
	~SolidSphere();
	void upload();
	void draw();
protected:
	vector<unsigned int> indices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> vertices;
	GLuint vbo[3];
};
#endif