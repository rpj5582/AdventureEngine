#version 430

struct Light
{
	vec4 position;
	vec3 color;
	float radius;
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

// Texture uniforms
layout (location = 8) uniform sampler2D theTexture;

// Lights
layout (location = 14) uniform Light lights[16];

// Output color
layout(location = 0) out vec4 finalFragColor;

vec3 calculateLighting(Light light, vec4 color)
{
	vec3 directionToLight = light.position.w == 0 ? normalize(-light.position.xyz) : normalize(light.position.xyz - position.xyz);
	float distanceToLight = length(light.position.xyz - position.xyz);

	// Calculates the diffuse lighting color
	vec3 diffuse = color.rgb * light.color * max(0, dot(normal, directionToLight));
	
	// Calculates the specular lighting color
	vec3 specularDirection = reflect(-directionToLight, normal);
	vec3 specular = light.color * pow(max(0, dot(specularDirection, cameraDirection)), specularDamping) * reflectivity;

	// Calculates the ambient lighting
	vec3 ambient = color.rgb * clamp(light.color, 0, 1) * 0.005f;

	// Calculates the attenuation
	float attenuationFactor =  light.position.w == 0 ? 0 : 1.0f / (light.radius * light.radius * 0.1f);
	float attenuation = 1.0f / (1 + attenuationFactor * distanceToLight * distanceToLight);

	// Calculates the final light color
	vec3 gamma = vec3(1 / 2.2f);
	vec3 finalLightingColor = pow((diffuse + specular) * attenuation + ambient, gamma);

	return finalLightingColor;
}

void main()
{
	// Gets the fragment color
	vec4 fragColor = texture(theTexture, uv);

	vec3 lightColor = vec3(0);
	for(int i = 0; i < lights.length(); i++)
	{
		lightColor += calculateLighting(lights[i], fragColor);
	}

	// Calcuates the final pixel color with fog
	finalFragColor = mix(vec4(skyColor, 1), fragColor * vec4(lightColor, 1), visibility);
}