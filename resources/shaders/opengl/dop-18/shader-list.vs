#version 300 es

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

in vec3 Position;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(Position, 1.0);
}