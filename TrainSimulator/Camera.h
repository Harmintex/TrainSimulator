#pragma once

#include <GL/glew.h>
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glfw3.h>

enum ECameraMovementType
{
	UNKNOWN,
	FORWARD,
	BACKWARD,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Camera
{
private:
	const float zNEAR = 0.1f;
	const float zFAR = 500.f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float FOV = 45.0f;
	glm::vec3 startPosition;

public:
	Camera(const int width, const int height, const glm::vec3& position);
	void Set(const int width, const int height, const glm::vec3& position);
	void Reset(const int width, const int height);
	void Reshape(int windowWidth, int windowHeight);

	const glm::vec3 GetPosition() const;
	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetProjectionMatrix() const;

	void ProcessKeyboard(ECameraMovementType direction, float deltaTime);
	void MouseControl(float xPos, float yPos);
	void ProcessMouseScroll(float yOffset);

	void ChangeXPosition(ECameraMovementType direction, float value);

private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVectors();

protected:
	const float cameraSpeedFactor = 30.0f;
	const float mouseSensitivity = 0.2f;

	float zNear;
	float zFar;
	float FoVy;
	int width;
	int height;
	bool isPerspective;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	bool bFirstMouseMove = true;
	float lastX = 0.f, lastY = 0.f;
};