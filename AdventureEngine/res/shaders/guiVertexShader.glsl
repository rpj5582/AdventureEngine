#version 430

// VAO Input
layout (location = 0) in vec2 position;

// Matrices used for position calculation
layout (location = 0) uniform mat4 modelMatrix;

// Texture atlas data
layout (location = 5) uniform uvec2 atlasSize;
layout (location = 6) uniform vec2 atlasOffset;

out vec2 uv;

void main()
{
	gl_Position = modelMatrix * vec4(position, 0, 1);
	uv = vec2((position.x + 1) / 2.0f, (position.y + 1) / 2.0f) / atlasSize + atlasOffset;
}