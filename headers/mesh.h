#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.h"

using std::string;
using std::vector;

#define MAX_BONE_INFLUENCE 4

/* Vertex data structre. */
struct Vertex {
	glm::vec3 Position; 
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
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
	unsigned int VAO;
	
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	/* Draw Call: Draws the corresponding mesh using the shader program passed to it as parameter. */
	void Draw(Shader& shader);

private:

	/* Render Data */
	unsigned int VBO, EBO;

	/* - Create buffer and vertex array objects. 
	   - Assign correct Vertex attribute pointers to the VAO */
	void setupMesh();

};

#endif