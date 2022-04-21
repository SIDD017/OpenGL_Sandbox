#include "camera.h"

Camera::Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float YAW, float PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(Speed), mouseSensitivity(Sensitivity), zoom(Zoom)
{
	position = glm::vec3(PosX, PosY, PosZ);
	worldUp = glm::vec3(UpX, UpY, UpZ);
	yaw = Yaw;
	pitch = Pitch;
	update_camera_vectors();
}

Camera::Camera(glm::vec3 Position, glm::vec3 Up, float YAW, float PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(Speed), mouseSensitivity(Sensitivity), zoom(Zoom)
{
	position = Position;
	worldUp = Up;
	yaw = Yaw;
	pitch = Pitch;
	update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == CAMERA_FORWARD) {
		position += front * velocity;
	}
	if (direction == CAMERA_BACKWARD) {
		position -= front * velocity;
	}
	if (direction == CAMERA_RIGHT) {
		position -= right * velocity;
	}
	if (direction == CAMERA_LEFT) {
		position += right * velocity;
	}
}

void Camera::process_mouse_movement(float xOffset, float yOffset, GLboolean contrainPitch)
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

void Camera::process_mouse_scroll(float yOffset)
{
	zoom -= (float)yOffset;
	if (zoom < 1.0f) {
		zoom = 1.0f;
	}
	if (zoom > 45.0f) {
		zoom = 45.0f;
	}
}

void Camera::update_camera_vectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(front, right));
}