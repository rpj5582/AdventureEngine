#version 430

// VAO Input
layout (location = 0) in vec3 position_in;

// Matrices used for position calculation
layout (location = 0) uniform mat4 viewMatrix;
layout (location = 1) uniform mat4 projectionMatrix;

out vec3 uv;

void main()
{
	gl_Position = (projectionMatrix * viewMatrix * vec4(position_in, 1)).xyww;
	uv = vec3(position_in.x, -position_in.yz);
}