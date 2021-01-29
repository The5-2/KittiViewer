#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera();
	~Camera();

	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;

	void setProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane);
	void setViewMatrix(const glm::vec3 eye, const glm::vec3 target, const glm::vec3 up);
	void updateViewMatrix();
	void printProperties();

	glm::vec3 position = glm::vec3(0.0f);   // position-vector
	glm::vec3 viewDir = glm::vec3(0.0f);    // viewing direction
	glm::vec3 upDir = glm::vec3(0.0f);      // up-vector
	glm::vec3 rightDir = glm::vec3(0.0f);   // right-vector (cross product of viewing- and up-direction)

	float delta = 1.0f;
	float mouseDelta = 1.0f;
};

#endif