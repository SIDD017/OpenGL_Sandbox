#version 330 core
out vec4 FragColor;

in vec4 ourcolor;
void main()
{
	FragColor = ourcolor;
}