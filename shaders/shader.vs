#version 330 core
// Input vertex attribute
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform vec3 posOffset;

out vec4 ourColor;
void main()
{
	gl_Position = vec4(aPos + posOffset, 1.0);
	ourColor = vec4(aColor, 1.0f);
}