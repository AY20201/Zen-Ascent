#version 430 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D renderedScene;

void main()
{
	vec3 col = texture(renderedScene, texCoord).rgb;
	FragColor = vec4(col, 1.0);
}