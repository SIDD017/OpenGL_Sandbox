#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"

const char* vertex_path = "shaders/shader.vs";
const char* fragment_path = "shaders/shader.fs";

/* Function prototypes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//Callback
void processInput(GLFWwindow* window);

/* glfw: whenever the window size changed (by OS or user resize) this callback function executes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/* process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly */
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(){

	/* Initialize GLFW */
	glfwInit();
	/* Tell GLFW to use OpenGL version 3.3 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/* Tell GLFW to use the core profile for OpenGL */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create Window Object */
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL_Project", NULL, NULL);
	if (window == NULL){
		/* Terminate program and print error message in case GLFW window is not created */
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	/* Change the current contextof OpenGL to the context just defined in window object */
	glfwMakeContextCurrent(window);

	/* Initialize GLAD before calling any OpenGL function */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	/* Register window resize callback */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glViewport(0, 0, 800, 600);

	float vertices[] = { -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
						 -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
						  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
						  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
						  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
						 -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };

	/** VBO and VAO */
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	/* NOTE: bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s) */
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);


	/* By using 0 we unbind the VBO from GL_ARRAY_BUFFER target.
	 * We are unbinding because the call to glVertexAttribPointer() ensured that 
	 * the VBO was registered as the vertex attribute's bound VBO */
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	Shader ourShader(vertex_path, fragment_path);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Wireframe Mode
	int nAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
	std::cout << "Maximum number of attributes supported = " << nAttributes << std::endl;

	/* RENDER LOOP: Keep application running till close window flag is not set */
	while (!glfwWindowShouldClose(window)) {

		/** Input events */
		processInput(window);

		/** Render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw the triangle */
		ourShader.use();	// Sets the specified shader program object as the active shader program
		glBindVertexArray(VAO);	// Binds the VAO. Any subsequent operations will now this VAO
		glDrawArrays(GL_TRIANGLES, 0, 6);	//	 Draws the vertices found in the currently bound VBO

		/** Check and call events an swap the buffers */
		/* Swaps color buffer for current render iteration */
		glfwSwapBuffers(window);
		/* Checks any poll events have been triggered (keyboard/mouse inputs, etc) */
		glfwPollEvents();
	}

	/* Deallocate all resources before terminating the application */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	/* Release allocated resources/emory and terminate application */
	glfwTerminate();

	return 0;
}