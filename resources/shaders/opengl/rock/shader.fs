#version 300 es

#ifdef GL_ES
	precision mediump float;
#endif

uniform sampler2D texture2D;

in  vec2 FragTextureCoordinates;

out vec4 FragOutput;

void main()
{
	//FragOutput = vec4(0.5, 0.5, 1.0, 1.0);
	
	FragOutput = texture(texture2D, FragTextureCoordinates);
}
