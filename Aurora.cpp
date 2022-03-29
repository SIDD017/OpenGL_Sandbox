#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"

const char* vertexPath = "shaders/shader.vs";
const char* fragmentPath = "shaders/shader.fs";
const char* fragmentPath_Yellow = "shaders/shader_yellow.fs";

/* Callbacks. */

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {

	/* Initialize GLFW and specify OpenGL version to 3.3 (core profile). */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a window object that holds all the required window information. */
	GLFWwindow* window = glfwCreateWindow(800, 600, "Aurora", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW Window\n";
		glfwTerminate();
		return -1;
	}
	/* Make the context of our window the main context on the current thread. */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* Initialize GLAD to get function pointers for OpenGL. */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	/* NOTE: Till here, we have :
	 * - Initialized GLFW and created a window object
	 * - Assigned the context of the window object as our main OpenGL context
	 * - Initialized GLAD to retrieve function pointers for OpenGL */

	/* Shader Compilation */

	/* Create a shader object. */
	Shader shader1(vertexPath, fragmentPath);
	Shader shader2(vertexPath, fragmentPath_Yellow);

	/* Number of available vertex attributes for current hardware configuration. */
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported : " << nrAttributes << std::endl;

	float vertices1[] = {
		/* First triangle */
		-0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	   -0.25f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	},
	vertices2[] = {
		 0.0f,  0.0f, 0.0f,
		0.25f,  0.5f, 0.0f,
		 0.5f,  0.0f, 0.0f,
	};

	/*unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};*/

	/* Generate a OpenGL buffer object and get a reference to it through an ID. */
	unsigned int VBO1, VBO2;
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &VBO2);

	/* Element Buffer Object */
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);

	unsigned int VAO1, VAO2;
	glGenVertexArrays(1, &VAO1);
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO1);

	/* Bind the newly created buffer object to the GL_ARRAY_BUFFER target.
	 * 
	 * After this any subsequent calls we make to the GL_ARRAY_BUFFER TYPE will 
	 * access the newly created VBO instead. */
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	/* Copy the vertex data into the currently bound buffer's memory */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Specify vertx attributes for the vertex shader. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	/* Since vertex attributes are disabled by default, we need to manually enable them. */
	glEnableVertexAttribArray(0);

	/* Specify vertx attributes for the vertex shader. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	/* Since vertex attributes are disabled by default, we need to manually enable them. */
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAO2);

	/* Bind the newly created buffer object to the GL_ARRAY_BUFFER target.
	 *
	 * After this any subsequent calls we make to the GL_ARRAY_BUFFER TYPE will
	 * access the newly created VBO instead. */
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	/* Copy the vertex data into the currently bound buffer's memory */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Specify vertx attributes for the vertex shader. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Since vertex attributes are disabled by default, we need to manually enable them. */
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* Render in wireframe mode. */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* Render loop */
	while(!glfwWindowShouldClose(window)){

		/* Input */
		processInput(window);

		/* Render commands */
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		shader1.use();
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		shader2.use();
		float greenVec[] = { 0.0f, greenValue, 0.0f, 1.0f };
		shader2.setVecN("vertColor", greenVec, 4);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		/* Check and call events and all buffers. */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	//glDeleteBuffers(1, &EBO);
	shader1.deleteProgram();
	shader2.deleteProgram();

	glfwTerminate();
	return 0;
}