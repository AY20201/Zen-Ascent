#version 430 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec4 currentPos;

uniform float nearClipPlane;
uniform float farClipPlane;

//uniform sampler2D depthSampler;

float LinearDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * nearClipPlane * farClipPlane) / (farClipPlane + nearClipPlane - z * (farClipPlane - nearClipPlane));
}

void main()
{
	vec2 deviceCoord = (currentPos.xy / currentPos.w) / 2.0 + 0.5;
	vec2 textureCoord = vec2(deviceCoord.x, deviceCoord.y);

	//float terrainDepth = texture(depthSampler, textureCoord).r;
	//float terrainDistance = LinearDepth(terrainDepth);

	float depth = LinearDepth(gl_FragCoord.z) / farClipPlane;
	FragColor = vec4(vec3(depth), 1.0);
}