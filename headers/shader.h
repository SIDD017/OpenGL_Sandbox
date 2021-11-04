#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:

	/* Identifier for the shader program object */
	unsigned int ID;

	/* Constructor: Reads and build the shader */
	Shader(const char *vertexPath, const char *fragmentPath);

	/* Use the shader */
	void use();

	/** Utility functions for setting Uniforms */
	/* NOTE: Here const at the end of the function prototype means that 
	 * this function cannot modify any of the member variables of this class. 
	 * 
	 * If it does then this will throw a compiler error */
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif