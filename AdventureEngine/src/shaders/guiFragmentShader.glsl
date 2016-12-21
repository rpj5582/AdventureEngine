#version 430

// The UV for this fragment
in vec2 uv;

// Texture uniform
layout (location = 8) uniform sampler2D theTexture;

// Output color
layout(location = 0) out vec4 finalFragColor;

void main()
{
	vec4 fragColor = texture(theTexture, uv);

	if(fragColor.a < 0.5f) discard;

	finalFragColor = fragColor;
}