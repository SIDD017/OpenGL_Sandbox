#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <iostream>

#include "shader.h"
#include "camera.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

/** Shaders */
const char* vertexPath = "shaders/shader.vs";
const char* fragmentPath = "shaders/shader.fs";
const char* fragmentPath_Yellow = "shaders/shader_yellow.fs";
const char* lightFragmentPath = "shaders/light_shader.fs";

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

unsigned int loadTexture(const char *texture_path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* textureData = stbi_load(texture_path, &width, &height, &nrChannels, 0);

	if (textureData) {
		GLenum image_format = GL_RGB;
		if (nrChannels == 1) {
			image_format = GL_RED;
		}
		else if (nrChannels == 3) {
			image_format = GL_RGB;
		}
		else if (nrChannels == 4) {
			image_format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, image_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture from file" << std::endl;
	}

	stbi_image_free(textureData);
	return texture;
}

int main() 
{
	/* Initialize GLFW and specify OpenGL version to 3.3 (core profile). */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	unsigned int texture1 = loadTexture(containertexture);
	unsigned int texture2 = loadTexture(containertexture_specular);

	/* Shader Compilation */

	/* Create a shader object. */
	Shader shader1(vertexPath, fragmentPath);
	Shader light_shader(vertexPath, lightFragmentPath);

	/* Number of available vertex attributes for current hardware configuration. */
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported : " << nrAttributes << std::endl;

	float vertices[] = {
		// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	/* Cube object positions */
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	/* Generate a OpenGL buffer object and get a reference to it through an ID. */
	unsigned int VBO1;
	glGenBuffers(1, &VBO1);

	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	/* Bind the newly created buffer object to the GL_ARRAY_BUFFER target.
	 * 
	 * After this any subsequent calls we make to the GL_ARRAY_BUFFER TYPE will 
	 * access the newly created VBO instead. */
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	/* Copy the vertex data into the currently bound buffer's memory */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Specify vertex attributes for the vertex shader. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	/* Since vertex attributes are disabled by default, we need to manually enable them. */
	glEnableVertexAttribArray(0);

	/* Specify vertex attributes for the vertex shader. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	/* Since vertex attributes are disabled by default, we need to manually enable them. */
	glEnableVertexAttribArray(1);

	/* Specify vertex attributes for the vertex shader. */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	/* Since vertex attributes are disabled by default, we need to manually enable them. */
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* Render in wireframe mode. */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* Enable Depth testing */
	glEnable(GL_DEPTH_TEST);
	
	shader1.use();
	shader1.setInt("material.diffuse", 0);
	shader1.setInt("material.specular", 1);

	/* Mouse input */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		/* Transformation */
		glm::mat4 view;
		glm::mat4 projection;
		view = camera.get_view_matrix();
		projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		float lightposition[3] = { camera.position.x, camera.position.y, camera.position.z };
		float viewerposition[3] = {camera.position.x, camera.position.y, camera.position.z};
		shader1.use();
		float material_ambient[3] = { 1.0f, 0.5f, 0.31f };
		float material_specular[3] = { 0.5f, 0.5f, 0.5f };
		float material_diffuse[3] = { 1.0f, 0.5f, 0.31f };
		float light_ambient[3] = { 0.2f, 0.2f, 0.2f };
		float light_specular[3] = { 1.0f, 1.0f, 1.0f };
		float light_diffuse[3] = { 0.5f, 0.5f, 0.5f };
		float light_direction[3] = { camera.front.x, camera.front.y, camera.front.z };
		shader1.setVecN("material.specular", material_specular, 3); 
		shader1.setVecN("light.ambient", light_ambient, 3);
		shader1.setVecN("light.specular", light_specular, 3);
		shader1.setVecN("light.diffuse", light_diffuse, 3);
		shader1.setFloat("material.shinniness", 32.0f);
		shader1.setVecN("light.direction", light_direction, 3);
		shader1.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
		shader1.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));
		shader1.setVecN("light.position", lightposition, 3);
		shader1.setVecN("viewPos", viewerposition, 3);
		/* Transformation Uniforms */
		int modelLoc = glGetUniformLocation(shader1.ID, "model");
		int viewLoc = glGetUniformLocation(shader1.ID, "view");
		int projectionLoc = glGetUniformLocation(shader1. ID, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO1);

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		/* Check and call events and all buffers. */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	shader1.deleteProgram();

	glfwTerminate();
	return 0;
}