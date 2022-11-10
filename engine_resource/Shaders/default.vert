#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 texCoord;
out vec3 normal;
out vec3 currentPos;
out vec4 currentPosLightSpace;
out vec3 tangent;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightMatrix;

void main()
{
	currentPos = vec3(modelMatrix * vec4(aPos, 1.0f));
	currentPosLightSpace = lightMatrix * vec4(currentPos, 1.0);
	gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);
	texCoord = aTex;
	normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
	tangent = aTangent;
}
