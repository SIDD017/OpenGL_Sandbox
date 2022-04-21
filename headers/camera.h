#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	CAMERA_FORWARD,
	CAMERA_BACKWARD,
	CAMERA_RIGHT,
	CAMERA_LEFT
};

const float Yaw = -90.0f;	//Initial values, facing the negative Z axis
const float Pitch = 0.0f;
const float Speed = 2.5f;
const float Sensitivity = 0.1f;
const float Zoom = 45.0f;

class Camera 
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw, pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	/* Constructor with Scalars */
	Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float YAW = Yaw, float PITCH = Pitch);

	/* Constructor with vectors */
	Camera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), float YAW = Yaw, float PITCH = Pitch);

	glm::mat4 get_view_matrix();

	void process_keyboard(Camera_Movement direction, float deltaTime);

	void process_mouse_movement(float xOffset, float yOffset, GLboolean contrainPitch = true);

	void process_mouse_scroll(float yOffset);

private:
	void update_camera_vectors();
};

#endif