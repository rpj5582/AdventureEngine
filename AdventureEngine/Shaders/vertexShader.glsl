#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvs;
layout (location = 2) in vec3 normals;

layout (location = 3) uniform mat4 mvpMatrix;

out vec2 uv;
out vec3 normal;

void main()
{
	gl_Position = mvpMatrix * vec4(position, 1);
	uv = uvs;
	normal = normals;
}