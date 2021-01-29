#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES

#include "../include/simpleGeometryCollection.h"
#include <iostream>
#include <map>

using namespace std;

void SimpleQuad::upload() {
	vertices = { glm::vec3(-1, -1, 0),
		glm::vec3(1, -1, 0),
		glm::vec3(1,  1, 0),
		glm::vec3(-1,  1, 0) };

	indices = { 0, 1, 3, 1, 2, 3 };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void SimpleQuad::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/***************************
 * 	LINE BOX
 * ************************/
 void LineBox::upload() {
		vertices = {
				glm::vec3(-0.5f, 0.f, 0.5f),
				glm::vec3(0.5f, 0.f, 0.5f),
				glm::vec3(-0.5f, 0.f, -0.5f),
				glm::vec3(0.5f, 0.f, -0.5f),
				glm::vec3(-0.5f, 1.f, 0.5f),
				glm::vec3(0.5f, 1.f, 0.5f),
				glm::vec3(-0.5f, 1.f, -0.5f),
				glm::vec3(0.5f, 1.f, -0.5f)}
				;

	indices = {	0, 1, 
				1, 3, 
				3, 2,
				2, 0,
				4, 5,
				5, 7,
				7, 6,
				6, 4,
				0, 4,
				1, 5,
				2, 6,
				3, 7 };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void LineBox::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/***************************
 * 	COORDINATE SYSTEM
 * ************************/
void CoordinateSystem::upload() {
	vertices = { 
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)};

	std::vector<glm::vec3> colors = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,         
        vertices.size() * sizeof(float) * 3, 
        &vertices[0].x, 
        GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,         
        colors.size() * sizeof(float) * 3, 
        &colors[0].x, 
        GL_STATIC_DRAW);
}

void CoordinateSystem::draw() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, vertices.size());

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/***************************
 * 	GRID XZ
 * ************************/
Grid_XZ::Grid_XZ(float distance, unsigned int numLines){
	m_distance = distance;
	m_numLines = numLines;
	m_lineLength = 2.0f * distance * numLines;
}

void Grid_XZ::upload() {
	unsigned int counter = 0;
	for(int i = -m_numLines; i <= int(m_numLines); i++){
		vertices.emplace_back(glm::vec3(float(i) * m_distance, 0.0f, 0.5f * m_lineLength));
		vertices.emplace_back(glm::vec3(float(i) * m_distance, 0.0f, -0.5f * m_lineLength));
		indices.push_back(counter++);
		indices.push_back(counter++);

		vertices.emplace_back(glm::vec3(0.5f * m_lineLength, 0.0f, float(i) * m_distance));
		vertices.emplace_back(glm::vec3(-0.5f * m_lineLength, 0.0f, float(i) * m_distance));
		indices.push_back(counter++);
		indices.push_back(counter++);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Grid_XZ::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/***************************
 * 	GRID YZ
 * ************************/
Grid_YZ::Grid_YZ(float distance, unsigned int numLines){
	m_distance = distance;
	m_numLines = numLines;
	m_lineLength = 2.0f * distance * numLines;
}

void Grid_YZ::upload() {
	unsigned int counter = 0;
	for(int i = -m_numLines; i <= int(m_numLines); i++){
		vertices.emplace_back(glm::vec3(0.0f, float(i) * m_distance, 0.5f * m_lineLength));
		vertices.emplace_back(glm::vec3(0.0f, float(i) * m_distance, -0.5f * m_lineLength));
		indices.push_back(counter++);
		indices.push_back(counter++);

		vertices.emplace_back(glm::vec3(0.0f, 0.5f * m_lineLength, float(i) * m_distance));
		vertices.emplace_back(glm::vec3(0.0f, -0.5f * m_lineLength, float(i) * m_distance));
		indices.push_back(counter++);
		indices.push_back(counter++);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Grid_YZ::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/***************************
 * 	SPHERE
 * ************************/
SolidSphere::SolidSphere(const float r, const int slices, const int stacks) {

	const float dTheta = 2.0*M_PI / (float)stacks;
	const float dPhi = M_PI / (float)slices;

	for (int i = stacks; i >= 0; i--) {
		glm::vec2 t(1 - i*dTheta / (2.0*M_PI), 1.0f);
		glm::vec3 p(0, r, 0);
		vertices.push_back(p);
		uvs.push_back(t);
	}

	const int tmpSize = vertices.size();

	//North pole
	for (int i = stacks; i >= 0; i--) {
		glm::vec2 t(1 - i*dTheta / (2.0*M_PI), (M_PI - dPhi) / M_PI);
		glm::vec3 p(r*sin(dPhi)*cos(i*dTheta), r*cos(dPhi), r*sin(dPhi)*sin(i*dTheta));
		vertices.push_back(p);
		uvs.push_back(t);
	}

	int triangleID = 0;
	for (; triangleID < stacks; triangleID++)
	{
		indices.push_back(triangleID);
		indices.push_back(triangleID + tmpSize);
		indices.push_back(triangleID + tmpSize + 1);
	}

	indices.push_back(stacks - 1);
	indices.push_back(stacks * 2 - 1);
	indices.push_back(stacks);


	// Middle Part 

	//	v0--- v2
	//  |  	/ |
	//  |  /  | 
	//  | /   |
	//  v1--- v3

	for (int j = 1; j<slices - 1; j++)
	{
		for (int i = stacks; i >= 0; i--)
		{
			glm::vec2 t = glm::vec2(1 - i*dTheta / (2.0*M_PI), (M_PI - (j + 1)*dPhi) / M_PI);
			glm::vec3 p = glm::vec3(r*sin((j + 1)*dPhi)*cos(i*dTheta), r*cos((j + 1)*dPhi), r*sin((j + 1)*dPhi)*sin(i*dTheta));
			vertices.push_back(p);
			uvs.push_back(t);

			//add two triangles

			indices.push_back(vertices.size() - stacks - 2);	//v0
			indices.push_back(vertices.size() - 1);			//v1
			indices.push_back(vertices.size() - stacks - 1);	//v2

			indices.push_back(vertices.size() - stacks - 1);	//v2
			indices.push_back(vertices.size() - 1);			//v1
			indices.push_back(vertices.size());			//v3

		}

	}

	const int lastVertex = vertices.size() - 1;

	//South Pole
	for (int i = stacks; i >= 0; i--) {
		glm::vec2 t(1 - i*dTheta / (2.0*M_PI), 0.0f);
		glm::vec3 p = glm::vec3(0, -r, 0);
		vertices.push_back(p);
		uvs.push_back(t);
	}

	triangleID = 0;
	for (; triangleID < stacks; triangleID++)
	{
		indices.push_back(lastVertex - stacks + triangleID);
		indices.push_back(lastVertex + triangleID + 1);
		indices.push_back(lastVertex - stacks + triangleID + 1);
	}

}

void SolidSphere::upload() {
	glGenBuffers(3, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float) * 2, uvs.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void SolidSphere::draw() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
