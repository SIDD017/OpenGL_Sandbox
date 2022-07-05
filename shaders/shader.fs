#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shinniness;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHTS 4

struct Light {
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// attenuation parameters
	//float constant;
	//float linear;
	//float quadratic;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) 
{
	vec3 lightDir = normalize(-light.direction);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	//combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) 
{
	vec3 lightDir = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	//combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

void main()
{
	//properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	
	//Directional Light
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	//Point Lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
	}

	FragColor = vec4(result, 1.0);

	/* vec3 lightDir = normalize(light.position - fragPos);
	
	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

	//diffuse
	vec3 norm = normalize(normal);
	//vec3 lightDir = normalize(light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texCoords)));

	//specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shinniness);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoords)));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	FragColor = vec4(ambient + diffuse + specular, 1.0); */
}