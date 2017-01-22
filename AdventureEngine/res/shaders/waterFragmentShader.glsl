#version 430

struct Light
{
	vec4 position;
	float intensity;
	float radius;
	vec3 color;
	vec3 coneDirection;
	float coneAngle;
};

// The position in world space, the position in clip space, and the uv for this fragment
in vec3 position;
in vec4 positionClipSpace;
in vec2 uv;

// The camera's position
in vec3 cameraPosition;

// Fog visibility (from 0 to 1)
in float visibility;

// Camera near and far planes
layout (location = 3) uniform float near;
layout (location = 4) uniform float far;

// Material properties
layout (location = 7) uniform float reflectivity;
layout (location = 8) uniform float specularDamping;

// Fog color
layout (location = 9) uniform vec3 fogColor;

// Texture uniforms
layout (location = 14) uniform sampler2D reflectionTexture;
layout (location = 15) uniform sampler2D refractionTexture;
layout (location = 16) uniform sampler2D depthMap;
layout (location = 17) uniform sampler2D dudvMap;
layout (location = 18) uniform sampler2D normalMap;

// The color of the water
layout (location = 21) uniform vec4 waterColor;

// How much the water's waves should be offset
layout (location = 22) uniform float waveMovement;

// Lights
layout (location = 23) uniform Light lights[16];

// Output color
layout(location = 0) out vec4 finalFragColor;

const float waveStrength = 0.005f;

vec3 calculateSpecularLighting(Light light, vec4 color, vec3 normal, float waterDepth)
{
	vec3 clampedLightColor = clamp(light.color, 0, 1);

	vec3 lightDirection = light.position.w == 0 ? normalize(light.position.xyz) : normalize(position - light.position.xyz);
	float distanceToLight = length(light.position.xyz - position);
	
	// Calculates the specular lighting color
	vec3 specularDirection = reflect(lightDirection, normal);
	vec3 specular = clampedLightColor * light.intensity * pow(max(0, dot(specularDirection, normalize(cameraPosition - position))), specularDamping) * reflectivity * clamp(waterDepth / 0.5f, 0, 1);

	// Calculates the attenuation
	float attenuationFactor =  light.position.w == 0 ? 0 : 1.0f / (light.radius * light.radius * 0.1f);
	float attenuation = 1.0f / (1 + attenuationFactor * distanceToLight * distanceToLight);

	// Calculates the spotlight attenuation
	float fragLightAngle = degrees(acos(dot(normalize(light.coneDirection), lightDirection)));
	if(fragLightAngle > light.coneAngle)
	{
		attenuation = 0;
	}

	// Calculates the final light color
	vec3 gamma = vec3(1 / 2.2f);
	vec3 finalLightingColor = pow((specular) * attenuation, gamma);

	return finalLightingColor;
}

void main()
{
	// Calculates the texture coordinates
	vec3 normalizedDeviceCoordinates = (positionClipSpace.xyz / positionClipSpace.w + 1) / 2.0f;
	vec2 reflectionUV = vec2(normalizedDeviceCoordinates.x, -normalizedDeviceCoordinates.y);
	vec2 refractionUV = normalizedDeviceCoordinates.xy;

	// Calculates the water's depth
	float depth = texture(depthMap, refractionUV).r;
	float distanceToGround = 2 * near * far / (far + near - (2 * depth - 1) * (far - near));

	depth = gl_FragCoord.z;
	float distanceToWaterSurface = 2 * near * far / (far + near - (2 * depth - 1) * (far - near));

	float waterDepth = distanceToGround - distanceToWaterSurface;

	// Calculates the distortion from the dudv map
	vec2 distortedUV = texture(dudvMap, vec2(uv.x + waveMovement, uv.y)).xy * 0.01f;
	distortedUV = uv + vec2(distortedUV.x, distortedUV.y + waveMovement);
	vec2 dudvOffset = (texture(dudvMap, distortedUV).xy * 2 - 1) * waveStrength * clamp(waterDepth, 0, 1);;

	// Calculate the normal from the normal map
	vec4 normalColor = texture(normalMap, distortedUV);
	vec3 normal = vec3(normalColor.r * 2 - 1, normalColor.b * 2, normalColor.g * 2 - 1);
	normal = normalize(normal);

	// Calculates the distorted reflection and refraction UVs
	vec2 distortedReflectionUV = reflectionUV + dudvOffset;
	distortedReflectionUV.x = clamp(distortedReflectionUV.x, 0.001f, 0.999f);
	distortedReflectionUV.y = clamp(distortedReflectionUV.y, -0.999f, 0.001f);

	vec2 distortedRefractionUV = refractionUV + dudvOffset;
	distortedRefractionUV = clamp(distortedRefractionUV, 0.001f, 0.999f);

	// Gets the reflection and refraction colors
	vec4 reflectionColor = texture(reflectionTexture, distortedReflectionUV);
	vec4 refractionColor = texture(refractionTexture, distortedRefractionUV);

	float mixFactor = dot(normalize(position - cameraPosition), normal);

	// Mixes the two together
	vec4 fragColor = mix(reflectionColor, refractionColor, mixFactor);

	// Adds a tint
	fragColor = mix(fragColor, waterColor, waterColor.w);

	vec3 lightColor = vec3(0);
	for(int i = 0; i < lights.length(); i++)
	{
		lightColor += calculateSpecularLighting(lights[i], fragColor, normal, waterDepth);
	}

	// Calcuates the final pixel color with fog
	finalFragColor = mix(vec4(fogColor, 1), fragColor + vec4(lightColor, 1), visibility);
	finalFragColor.a = clamp(waterDepth / 0.5f, 0, 1);
}