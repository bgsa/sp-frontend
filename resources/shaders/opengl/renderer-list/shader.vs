#version 300 es

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform samplerBuffer transformAsTexture;

in  vec3 Position;

void main()
{
	vec4 col1 = texelFetch(transformAsTexture, gl_InstanceID * 4);
	vec4 col2 = texelFetch(transformAsTexture, gl_InstanceID * 4 + 1);
	vec4 col3 = texelFetch(transformAsTexture, gl_InstanceID * 4 + 2);
	vec4 col4 = texelFetch(transformAsTexture, gl_InstanceID * 4 + 3);
	mat4 transform = mat4(col1, col2, col3, col4);

	gl_Position = projectionMatrix * viewMatrix * transform * vec4(Position, 1.0);
}