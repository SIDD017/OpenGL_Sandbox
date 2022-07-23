#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720

/** Shaders */
const char* vertexPath = "shaders/shader.vs";
//const char* fragmentPath = "shaders/shader.fs";
const char* fragmentPath = "shaders/light_shader.fs";

const string modelPath = "models/backpack.obj";

/** Textures */
const char* woodenTexture = "textures/container.jpg";
const char* faceTexture = "textures/awesomeface.png";
const char* coolTexture = "textures/cool.png";
const char* dvdtexture = "textures/dvd.png";
const char* containertexture = "textures/container2.png";
const char* containertexture_specular = "textures/container2_specular.png";

/* Camera */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));	
/* Defaults */
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool first_mouse = true;	

/** Callbacks. */

void framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.process_keyboard(CAMERA_FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.process_keyboard(CAMERA_BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.process_keyboard(CAMERA_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.process_keyboard(CAMERA_RIGHT, deltaTime);
	}
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos) 
{
	/* Reset position if mouse control is captured for the first time since the application was run. */
	if (first_mouse) {
		lastX = static_cast<float>(xPos);
		lastY = static_cast<float>(yPos);
		first_mouse = false;
	}

	float xOffset = static_cast<float>(xPos) - lastX;
	float yOffset = lastY - static_cast<float>(yPos);
	lastX = static_cast<float>(xPos);
	lastY = static_cast<float>(yPos);

	camera.process_mouse_movement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) 
{
	camera.process_mouse_scroll(static_cast<float>(yoffset));
}

int main() 
{
	/* Initialize GLFW and specify OpenGL version to 3.3 (core profile). */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a window object that holds all the required window information. */
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Aurora", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW Window\n";
		glfwTerminate();
		return -1;
	}
	/* Make the context of our window the main context on the current thread. */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/* Capture the mouse */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Initialize GLAD to get function pointers for OpenGL. */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	/* NOTE: Till here, we have :
	 * - Initialized GLFW and created a window object
	 * - Assigned the context of the window object as our main OpenGL context
	 * - Initialized GLAD to retrieve function pointers for OpenGL */

	 /* Texture loading */
	stbi_set_flip_vertically_on_load(true);

	/* Render in wireframe mode. */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* Enable Depth testing */
	glEnable(GL_DEPTH_TEST);

	Shader shader(vertexPath, fragmentPath);

	Model ourModel(modelPath);

	/* Render loop */
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/* Input */
		processInput(window);

		/* Render commands */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.get_view_matrix();
		int projectionLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(shader);

		/* Check and call events and all buffers. */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.deleteProgram();

	glfwTerminate(); 
	return 0;
}