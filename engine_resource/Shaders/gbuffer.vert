#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelMatrix;

void main()
{
	vec4 viewpos = view * model * vec4(aPos, 1.0);
	fragPos = viewPos.xyz;
	texCoord = aTex;

	normal = mat3(transpose(inverse(view * modelMatrix))) * aNormal;

	gl_Position = projection * viewpos;
}