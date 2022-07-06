#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.h"

using std::string;
using std::vector;

struct Vertex {
	glm::vec3 Position; 
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
};

class Mesh {
public:
	/* Mesh Data */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void Draw(Shader& shader);

private:

	/* Render Data */
	unsigned int VAO, VBO, EBO;

	/* - Create buffer and vertex array objects. 
	   - Assign correct Vertex attribute pointers to the VAO */
	void setupMesh();

};

#endif