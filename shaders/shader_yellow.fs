#version 330 core

uniform vec4 vertColor;
out vec4 FragColor;

void main()
{
	FragColor = vertColor;
}