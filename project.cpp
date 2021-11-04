#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/* Shaders */
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	color = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 color;\n"
"void main()\n"
"{\n"
"	FragColor = color;\n"
"}\0";

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

	/** Vertex Shader */
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);	// Create shader object and specify the type of shader
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);	// Bind the shader sourcecode(GLSL) to the shader object
	glCompileShader(vertex_shader);	// Compile the shader
	/* Check if the shader compiled correctly */
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	/** Fragment Shader */
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);	// Create shader object and specify type
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);	// Bind shader sourcecode (GLSL) to the shader object
	glCompileShader(fragment_shader);	// Compile the shader
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	/** Link Shaders to a shader program */
	unsigned int shader_program = glCreateProgram();	// Create shader program object
	glAttachShader(shader_program, vertex_shader);	// First attach the vertex shader
	glAttachShader(shader_program, fragment_shader);	// Attach the fragment shader after the veretx shader
	glLinkProgram(shader_program);	// Link the shaders that have been attached to the shader program
	/* Check for linking errors in the shader program */
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	/* Once shader program is created the original shader objects can be deleted */
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

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
		glUseProgram(shader_program);	// Sets the specified shader program object as the active shader program
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
	glDeleteProgram(shader_program);

	/* Release allocated resources/emory and terminate application */
	glfwTerminate();

	return 0;
}