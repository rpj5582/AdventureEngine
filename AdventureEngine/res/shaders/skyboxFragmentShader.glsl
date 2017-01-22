#version 430

// The uv for this fragment
in vec3 uv;

// Fog color
layout (location = 9) uniform vec3 fogColor;

// Fog gradient start and end points
layout (location = 12) uniform float lowerFogGradient;
layout (location = 13) uniform float upperFogGradient;

// Texture uniform
layout (location = 14) uniform samplerCube cubeMap;

// Output color
layout(location = 0) out vec4 finalFragColor;

void main()
{
	vec4 skyboxColor = texture(cubeMap, uv);

	float fogFactor = clamp((-uv.y - lowerFogGradient) / (upperFogGradient - lowerFogGradient), 0, 1);

	finalFragColor = mix(vec4(fogColor, 1), skyboxColor, fogFactor);
}