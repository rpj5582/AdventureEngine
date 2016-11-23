#version 430

in vec2 uv;
in vec3 normal;

uniform sampler2D theTexture;

void main()
{
	gl_FragColor = texture(theTexture, uv);
}