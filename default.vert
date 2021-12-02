#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 normal;
out vec3 currentPos;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
	currentPos = vec3(modelMatrix * vec4(aPos, 1.0f));
	gl_Position = camMatrix * vec4(aPos, 1.0);
	texCoord = aTex;
	normal = aNormal;
}
