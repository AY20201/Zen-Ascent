#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

#define PI 3.14159

out vec2 texCoord;
out vec3 normal;
out vec4 currentPos;
out vec3 tangent;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

uniform float elapsedTime;

vec3 GerstnerWave(vec4 wave, vec3 p, inout vec3 tangent, inout vec3 binormal)
{
	float steepness = wave.z;
    float wavelength = wave.w;
    float k = 2 * PI / wavelength;
    float c = sqrt(9.81 / k);
    float2 d = normalize(wave.xy);
    float f = k * (dot(d, p.xz) - c * elapsedTime);
    float a = steepness / k;

    tangent += vec3(-d.x * d.x * (steepness * sin(f)), d.x * (steepness * cos(f)), -d.x * d.y * (steepness * sin(f)));

    binormal += vec3(-d.x * d.y * (steepness * sin(f)), d.y * (steepness * cos(f)), -d.y * d.y * (steepness * sin(f)));

    return vec3(d.x * (a * cos(f)), a * sin(f), d.y * (a * cos(f)));

}

void main()
{
    vec3 vPos = aPos;
    vec3 tangent = vec3(1.0f, 0.0f, 0.0f);
    vec3 binormal = vec3(0.0f, 0.0f, 1.0f);
    p += GerstnerWave(vec4(), aPos, tangent, binormal);
    normal = normalize(cross(binormal, tangent));

	currentPos = modelMatrix * vec4(aPos, 1.0f));
	gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);
	texCoord = aTex;
    tangent = aTangent;
}