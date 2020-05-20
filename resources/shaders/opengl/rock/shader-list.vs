#version 300 es

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix[2];

in vec3 Position;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transformMatrix[gl_InstanceID] * vec4(Position, 1.0);
}