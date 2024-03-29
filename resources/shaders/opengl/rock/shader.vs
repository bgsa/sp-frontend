#version 300 es

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in  vec3 Position;
in  vec2 TexCoord;

out vec2 FragTexCoord;

void main()
{
	FragTexCoord = TexCoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(Position, 1.0);
}