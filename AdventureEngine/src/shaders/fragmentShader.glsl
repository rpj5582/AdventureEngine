#version 430

struct Light
{
	vec4 position;
	float intensity;
	float radius;
	vec3 color;
	vec3 coneDirection;
	float coneAngle;
	float falloff;
};

// The position, UV and normal vector for this fragment
in vec3 position;
in vec2 uv;
in vec3 normal;

// Direction and distance to light and camera's position, used for specular lighting calculation
in vec3 directionToLight;
in float distanceToLight;
in vec3 cameraDirection;

// Fog visibility (from 0 to 1)
in float visibility;

// Material properties
layout (location = 5) uniform float reflectivity;
layout (location = 6) uniform float specularDamping;

// Sky color, used for fog
layout (location = 7) uniform vec3 skyColor;

// Texture uniform
layout (location = 8) uniform sampler2D theTexture;

// Lights
layout (location = 14) uniform Light lights[16];

// Output color
layout(location = 0) out vec4 finalFragColor;

vec3 calculateLighting(Light light, vec4 color)
{
	vec3 clampedLightColor = clamp(light.color, 0, 1);

	vec3 lightDirection = light.position.w == 0 ? normalize(light.position.xyz) : normalize(position.xyz - light.position.xyz);
	float distanceToLight = length(light.position.xyz - position.xyz);

	// Calculates the diffuse lighting color
	vec3 diffuse = color.rgb * clampedLightColor * light.intensity * max(0.0005f, dot(normal, -lightDirection));
	
	// Calculates the specular lighting color
	vec3 specularDirection = reflect(lightDirection, normal);
	vec3 specular = clampedLightColor * light.intensity * pow(max(0, dot(specularDirection, cameraDirection)), specularDamping) * reflectivity;

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
	vec3 finalLightingColor = pow((diffuse + specular) * attenuation, gamma);

	return finalLightingColor;
}

void main()
{
	// Gets the fragment color
	vec4 fragColor = texture(theTexture, uv);

	if(fragColor.a < 0.5f) discard;

	vec3 lightColor = vec3(0);
	for(int i = 0; i < lights.length(); i++)
	{
		lightColor += calculateLighting(lights[i], fragColor);
	}

	// Calcuates the final pixel color with fog
	finalFragColor = mix(vec4(skyColor, 1), fragColor * vec4(lightColor, 1), visibility);
}