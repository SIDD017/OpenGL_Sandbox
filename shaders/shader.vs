#version 330 core
// Input vertex attribute
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 transform;

out vec4 ourColor;
out vec2 texCoords;
void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	ourColor = vec4(aColor, 1.0f);
	texCoords = aTexCoords;
}