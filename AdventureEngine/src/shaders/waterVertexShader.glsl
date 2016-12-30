#version 430

// VAO Input
layout (location = 0) in vec3 position_in;

// Matrices used for position calculation
layout (location = 0) uniform mat4 modelMatrix;
layout (location = 1) uniform mat4 viewMatrix;
layout (location = 2) uniform mat4 projectionMatrix;

out vec3 position;
out vec4 positionClipSpace;
out vec2 uv;
out vec3 normal;

out vec3 cameraPosition;

out float visibility;

// Fog constants
const float density = 0.005f;
const float gradient = 1.0f;

void main()
{
	vec4 worldPosition = modelMatrix * vec4(position_in, 1);
	vec4 positionCameraSpace = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionCameraSpace;

	position = worldPosition.xyz;
	positionClipSpace = gl_Position;
	uv = (position_in.xz + 1) / 2.0f;

	// Calculates the camera's position, used for lighting calculations in the fragment shader
	cameraPosition = (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz;

	// Calculates the visibility factor for fog
	float distance = length(positionCameraSpace.xyz);
	visibility = exp(-pow(distance * density, gradient));
}