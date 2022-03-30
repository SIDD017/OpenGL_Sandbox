#version 330 core
in vec4 ourColor;
in vec2 texCoords;

uniform sampler2D sourceTexture1;
uniform sampler2D sourceTexture2;

out vec4 FragColor;

void main()
{
	FragColor = texture(sourceTexture2, texCoords);
}