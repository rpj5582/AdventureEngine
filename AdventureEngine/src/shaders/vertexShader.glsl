#version 430

struct Light
{
	vec4 position;
	vec3 color;
	float radius;
};

// VAO Input
layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 uv_in;
layout (location = 2) in vec3 normal_in;

// Matrices used for position calculation
layout (location = 0) uniform mat4 modelMatrix;
layout (location = 1) uniform mat4 viewMatrix;
layout (location = 2) uniform mat4 projectionMatrix;

// Texture atlas data
layout (location = 3) uniform uvec2 atlasSize;
layout (location = 4) uniform vec2 atlasOffset;

out vec3 position;
out vec2 uv;
out vec3 normal;

out vec3 cameraDirection;

out float visibility;

// Fog constants
const float density = 0.005f;
const float gradient = 5.0f;

void main()
{
	vec4 worldPosition = modelMatrix * vec4(position_in, 1); // Position in world space
	vec4 positionCameraSpace = viewMatrix * worldPosition; // Position in camera space
	gl_Position = projectionMatrix * positionCameraSpace; // Final position

	position = worldPosition.xyz;

	// Calculates the correct uv when considering texture atlases
	uv = uv_in / atlasSize + atlasOffset;

	// Calculates the normal in world space
	normal = normalize(modelMatrix * vec4(normal_in, 0)).xyz;

	// Calculates the light's direction, distance and the camera's direction, used for lighting calculations in the fragment shader
	cameraDirection = normalize((inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz);

	// Calculates the visibility factor for fog
	float distance = length(positionCameraSpace.xyz);
	visibility = exp(-pow(distance * density, gradient));
}