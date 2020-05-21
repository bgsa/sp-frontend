#version 300 es

#ifndef LIST_LENGTH
	#define LIST_LENGTH 100
#endif

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix[ LIST_LENGTH ];

in vec3 Position;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transformMatrix[gl_InstanceID] * vec4(Position, 1.0);
}