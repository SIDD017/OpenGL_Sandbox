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
	Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float YAW = Yaw, float PITCH = Pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(Speed), mouseSensitivity(Sensitivity), zoom(Zoom) 
	{
		position = glm::vec3(PosX, PosY, PosZ);
		worldUp = glm::vec3(UpX, UpY, UpZ);
		yaw = Yaw;
		pitch = Pitch;
		update_camera_vectors();
	}

	/* Constructor with vectors */
	Camera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), float YAW = Yaw, float PITCH = Pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(Speed), mouseSensitivity(Sensitivity), zoom(Zoom)
	{
		position = Position;
		worldUp = Up;
		yaw = Yaw;
		pitch = Pitch;
		update_camera_vectors();
	}

	glm::mat4 get_view_matrix()
	{
		return glm::lookAt(position, position + front, up);
	}

	void process_keyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;
		if (direction == CAMERA_FORWARD) {
			position += front * velocity;
		}
		if (direction == CAMERA_BACKWARD) {
			position -= front * velocity;
		}
		if (direction == CAMERA_RIGHT) {
			position += right * velocity;
		}
		if (direction == CAMERA_LEFT) {
			position -= right * velocity;
		}
	}

	void process_mouse_movement(float xOffset, float yOffset, GLboolean contrainPitch = true)
	{
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		/* Angle constraints */
		if (contrainPitch) {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			if (pitch < -89.0f) {
				pitch = -89.0f;
			}
		}

		update_camera_vectors();
	}

	void process_mouse_scroll(float yOffset)
	{
		zoom -= (float)yOffset;
		if (zoom < 1.0f) {
			zoom = 1.0f;
		}
		if (zoom > 45.0f) {
			zoom = 45.0f;
		}
	}

private:
	void update_camera_vectors()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(direction);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(front, right));
	}
};

#endif