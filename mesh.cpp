#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::setupMesh()
{
	/* Create Vertex Array Object and VBO, EBO buffer objects, and store a reference to their corresponding IDs. */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/* Bind VAO to setup Vertex Attribute Pointers. */
	glBindVertexArray(VAO);

	/* Bind VBO as the cuurent active buffer object. */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/* Load vertex data into memory and bind to active buffer object */
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	/* Bind EBO to load polygon indices from vertex data. */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	/* Set correct vertex attribute pointers as specified in the vertex shader. */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	/* Unbind the VAO. To be used later during Draw Call. */
	glBindVertexArray(0);

}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id); 
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}