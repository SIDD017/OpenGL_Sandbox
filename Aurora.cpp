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
//#include "model.h"
#include <map>
#include <model.h>
#include <random>

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720

/** Shaders */
const char* vertexPath = "shaders/shader.vs";
const char* singleColorFragmentPath = "shaders/single_color_shader.fs";
const char* fragmentPath = "shaders/light_shader.fs";
const char* quadVertexPath = "shaders/quad.vs";
const char* quadFragmentPath = "shaders/quad.fs";
const char* skyboxVertexPath = "shaders/cubemap.vs";
const char* skyboxFragmentPath = "shaders/cubemap.fs";
const char* simpleVertexPath = "shaders/simple.vs";
const char* simpleGeometryPath = "shaders/simple.gs";
const char* simpleFragmentPath = "shaders/simple.fs";
const char* simpleDepthVertexPath = "shaders/simpleDepth.vs";
const char* simpleDepthGeometryPath = "shaders/simpleDepth.gs";
const char* simpleDepthFragmentPath = "shaders/simpleDepth.fs";
const char* normalVertexPath = "shaders/normal.vs";
const char* normalFragmentPath = "shaders/normal.fs";
const char* normalGeometryPath = "shaders/normal.gs";
const char* simpleShadowVertexPath = "shaders/shadow.vs";
const char* simpleShadowFragmentPath = "shaders/shadow.fs";
const char* normalMappingVertexPath = "shaders/normal_map.vs";
const char* normalMappingFragmentPath = "shaders/normal_map.fs";
const char* parallaxMappingVertexPath = "shaders/parallax_map.vs";
const char* parallaxMappingFragmentPath = "shaders/parallax_map.fs";
const char* lightingVertexPath = "shaders/lighting.vs";
const char* lightingFragmentPath = "shaders/lighting.fs";
const char* hdrVertexPath = "shaders/hdr.vs";
const char* hdrFragmentPath = "shaders/hdr.fs";
const char* bloomVertexPath = "shaders/bloom.vs";
const char* bloomFragmentPath = "shaders/bloom.fs";
const char* lightboxFragmentPath = "shaders/light_box.fs";
const char* blurVertexPath = "shaders/blur.vs";
const char* blurFragmentPath = "shaders/blur.fs";
const char* bloomfinalVertexPath = "shaders/bloom_final.vs";
const char* bloomfinalFragmentPath = "shaders/bloom_final.fs";
const char* bufferVertexPath = "shaders/buffer.vs";
const char* bufferFragmentPath = "shaders/buffer.fs";
const char* defferedShadingVertexPath = "shaders/deferred_shading.vs";
const char* defferedShadingFragmentPath = "shaders/deferred_shading.fs";
const char* defferedLightBoxVertexPath = "shaders/deferred_light_box.vs";
const char* defferedLightBoxFragmentPath = "shaders/deferred_light_box.fs";
const char* ssaoGeometryVertexPath = "shaders/ssao_geometry.vs";
const char* ssaoGeometryFragmentPath = "shaders/ssao_geometry.fs";
const char* ssaoVertexPath = "shaders/ssao.vs";
const char* ssaoLightingFragmentPath = "shaders/ssao_lighting.fs";
const char* ssaoFragmentPath = "shaders/ssao.fs";
const char* ssaoBlurFragmentPath = "shaders/ssao_blur.fs";
const char* pbrVertexPath = "shaders/pbr.vs";
const char* pbrFragmentPath = "shaders/pbr.fs";

/* Models. */
const string modelPath = "models/backpack.obj";
const string rockPath = "models/rock.obj";
const string planetPath = "models/planet.obj";

/** Textures */
const char* woodenTexture = "textures/container.jpg";
const char* faceTexture = "textures/awesomeface.png";
const char* coolTexture = "textures/cool.png";
const char* dvdtexture = "textures/dvd.png";
const char* wallTexture = "textures/wall.jpg";
const char* vegetationTexture = "textures/grass.png";
const char* transparent_window_texture = "textures/blending_transparent_window.png";
const char* containertexture = "textures/container2.png";
const char* containertexture_specular = "textures/container2_specular.png";
const char* woodtexture = "textures/wood.png";
const char* brickwall_diffuse = "textures/brickwall.jpg";
const char* brickwall_normal = "textures/brickwall_normal.jpg";
const char* brick2_diffuse = "textures/bricks2.jpg";
const char* brick2_normal = "textures/bricks2_normal.jpg";
const char* brick2_displacement = "textures/bricks2_disp.jpg";


/* NOTE: vector initialization should be done during declaration itself. C++ doesn't allow 
 * assignment operations (or any operation tbh) outside a class/function. */

/* Cubemap textures. */
vector<string> cubemapFaces{
	"textures/skybox/right.jpg",
	"textures/skybox/left.jpg",
	"textures/skybox/top.jpg",
	"textures/skybox/bottom.jpg",
	"textures/skybox/front.jpg",
	"textures/skybox/back.jpg"
};

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

/* Utility function for loading a 2D texture from file. */
unsigned int loadTexture(char const* path, bool gammaCorrection)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}


int main()
{
	/* Initialize GLFW and specify OpenGL version to 3.3 (core profile). */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* MSAA using a 4 subsample buffer per pixel. */
	//glfwWindowHint(GLFW_SAMPLES, 4);

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

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader shader(pbrVertexPath, pbrFragmentPath);

	shader.use();
	float temp[3] = { 0.5f, 0.0f, 0.0f };
	shader.setVecN("albedo", temp, 3);
	shader.setFloat("ao", 1.0f);

	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader.use();
	int projectionloc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projectionloc, 1, GL_FALSE, glm::value_ptr(projection));

	/* Render loop */
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/* Input */
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 view = camera.get_view_matrix();
		int viewloc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
		float tempcampos[3] = { camera.position.x, camera.position.y, camera.position.z };
		shader.setVecN("camPos", tempcampos, 3);

		// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
		glm::mat4 model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row)
		{
			shader.setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				shader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				int modelloc = glGetUniformLocation(shader.ID, "model");
				glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
				renderSphere();
			}
		}

		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			float temp1[3] = { newPos.x, newPos.y, newPos.z };
			shader.setVecN("lightPositions[" + std::to_string(i) + "]", temp1, 3);
			float temp2[3] = { lightColors[i].x, lightColors[i].y, lightColors[i].z };
			shader.setVecN("lightColors[" + std::to_string(i) + "]", temp2, 3);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			int modelloc = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
			renderSphere();
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();  
	return 0;
}