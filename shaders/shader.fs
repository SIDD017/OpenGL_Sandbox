#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shinniness;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

	//diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texCoords)));

	//specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shinniness);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoords)));

	//final color
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}