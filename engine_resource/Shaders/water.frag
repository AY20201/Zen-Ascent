#version 430 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec4 currentPos;
in vec3 tangent;

uniform float nearClipPlane;
uniform float farClipPlane;

uniform vec3 camPos;
uniform sampler2D depthSampler;

void main()
{
	vec2 deviceCoord = (currentPos.xy / currentPos.w) / 2.0 + 0.5;

	float terrainDepth = texture(depthSampler, vec2(deviceCoord.x, deviceCoord.y)).r;
	vec3 viewDir = normalize(camPos - currentPos);

	FragColor = vec4(vec3(depth), 1.0);
}