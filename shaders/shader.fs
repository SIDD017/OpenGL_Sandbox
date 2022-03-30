#version 330 core
in vec4 ourColor;
in vec2 texCoords;

uniform sampler2D sourceTexture;

out vec4 FragColor;

void main()
{
	FragColor = texture(sourceTexture, texCoords);
}