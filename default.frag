#version 330 core
out vec4 FragColor;
in vec3 color;
in vec2 texCoord;
//in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec4 lightColor;

void main()
{
	/*
	//diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseColor = diff * lightColor;
	*/

	FragColor = texture(tex, texCoord)/* * lightColor * diffuseColor*/;
}