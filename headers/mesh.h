#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.h"

using std::string;
using std::vector;

/* Vertex data structre. */
struct Vertex {
	glm::vec3 Position; 
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

/* Stores a reference to the loaded tecture through id and stores its type as a string. */
struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	/* Mesh Data */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	/* Draw Call: Draws the corresponding mesh using the shader program passed to it as parameter. */
	void Draw(Shader& shader);

private:

	/* Render Data */
	unsigned int VAO, VBO, EBO;

	/* - Create buffer and vertex array objects. 
	   - Assign correct Vertex attribute pointers to the VAO */
	void setupMesh();

};

#endif