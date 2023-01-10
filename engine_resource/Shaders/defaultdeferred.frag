#version 440 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gLightPosition;
layout (location = 2) out vec3 gNormal;
layout (location = 3) out vec4 gAlbedo;

in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;
in vec4 currentPosLightSpace;
in vec3 tangent;

uniform sampler2D albedo;
uniform float albedoScale;
uniform vec3 albedoColor;
uniform sampler2D normalMap;
uniform float normalMapScale;

float lightingFactor = 1.0;

void main()
{
	vec3 norm = normalize(normal);

	vec3 normalMapSample = (texture(normalMap, texCoord * normalMapScale).rgb * 2.0 - 1.0) * normalMapStrength;
	vec3 tan = normalize(tangent);
	tan = normalize(tan - dot(tan, norm) * norm);
	vec3 bitangent = cross(tan, norm);
	mat3 TBN = mat3(tan, bitangent, norm);
	norm = normalize(TBN * normalMapSample);

	gPosition = currentPos;
	gLightPosition = currentPosLightSpace;
	gNormal = norm;
	gAlbedo.rgb = texture(albedo, texCoord * albedoScale).rgb * albedoColor;
	gAlbedo.a = lightingFactor;
}