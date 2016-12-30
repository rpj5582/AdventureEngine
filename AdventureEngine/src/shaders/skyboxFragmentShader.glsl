#version 430

// The uv for this fragment
in vec3 uv;

// Fog color
layout (location = 7) uniform vec3 fogColor;

// Texture uniform
layout (location = 8) uniform samplerCube cubeMap;

// Output color
layout(location = 0) out vec4 finalFragColor;

const float lowerFogGradient = 0;
const float upperFogGradient = 0.25f;

void main()
{
	vec4 skyboxColor = texture(cubeMap, uv);

	float fogFactor = clamp((-uv.y - lowerFogGradient) / (upperFogGradient - lowerFogGradient), 0, 1);

	finalFragColor = mix(vec4(fogColor, 1), skyboxColor, fogFactor);
}