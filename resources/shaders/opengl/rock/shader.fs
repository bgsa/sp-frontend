#version 300 es
precision mediump float;

uniform sampler2D tex;

in  vec2 FragTexCoord;

out vec4 FragOutput;

void main()
{
	//FragOutput = vec4(0.5, 0.5, 1.0, 1.0);
	FragOutput = texture(tex, FragTexCoord);
}