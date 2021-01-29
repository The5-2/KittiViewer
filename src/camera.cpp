#include "../include/camera.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::setProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane) {
	projMatrix = glm::perspective(fovy, aspect, nearPlane, farPlane);
}

void Camera::setViewMatrix(const glm::vec3 eye, const glm::vec3 target, const glm::vec3 up) {
	this->position = eye;  
	this->viewDir = target - eye;
	this->upDir = up;     
	this->rightDir = glm::normalize(glm::cross(glm::vec3(viewDir), glm::vec3(upDir)));
	updateViewMatrix();
}

glm::mat4 lookAt_selfmade(const glm::vec3& Eye, const glm::vec3& Center, const glm::vec3& Up){
    glm::mat4 Matrix;
   	glm::vec3 X, Y, Z;

	Z = Eye - Center;
	std::cout << "Z: " << glm::to_string(Z) << std::endl;
    Z = glm::normalize(Z);
	std::cout << "Z: " << glm::to_string(Z) << std::endl;

    Y = Up;
	std::cout << "Y: " << glm::to_string(Y) << std::endl;

    X = glm::cross(Y, Z);
	std::cout << "X: " << glm::to_string(X) << std::endl;

	X = glm::normalize(X);
	std::cout << "X: " << glm::to_string(X) << std::endl;

	Y = glm::normalize(Y);
	std::cout << "Y: " << glm::to_string(Y) << std::endl;

    Matrix[0][0] = X.x;
    Matrix[1][0] = X.y;
    Matrix[2][0] = X.z;
    Matrix[3][0] = glm::dot(-X, Eye );
    Matrix[0][1] = Y.x;
    Matrix[1][1] = Y.y;
    Matrix[2][1] = Y.z;
    Matrix[3][1] = glm::dot(-Y, Eye );
    Matrix[0][2] = Z.x;
    Matrix[1][2] = Z.y;
    Matrix[2][2] = Z.z;
    Matrix[3][2] = glm::dot(-Z, Eye );
    Matrix[0][3] = 0;
    Matrix[1][3] = 0;
    Matrix[2][3] = 0;
    Matrix[3][3] = 1.0f;

	std::cout << "Matrix: " << glm::to_string(Matrix) << std::endl;

    return Matrix;
}

void Camera::updateViewMatrix(){
	viewMatrix = glm::lookAt(position, position + viewDir, upDir);
	// viewMatrix = lookAt_selfmade(position, position + viewDir, upDir);
}

void Camera::printProperties() {
	std::cout << "V: " << glm::to_string(viewMatrix) << std::endl;
	std::cout << "Pos: " << glm::to_string(position) << std::endl;
	std::cout << "ViewDir: " << glm::to_string(viewDir) << std::endl;
	std::cout << "UpDir: " << glm::to_string(upDir) << std::endl;
	std::cout << "RightDir: " << glm::to_string(rightDir) << std::endl;
	std::cout << "Deltas: " << delta << ", " << mouseDelta << std::endl;
}
