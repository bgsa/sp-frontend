#version 300 es

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform samplerBuffer transformMatrix;

in vec3 Position;
in vec3 Normal;

out vec3 normalCoord, eyeCoord;

void main()
{
	vec4 col1 = texelFetch(transformMatrix, gl_InstanceID * 4);
	vec4 col2 = texelFetch(transformMatrix, gl_InstanceID * 4 + 1);
	vec4 col3 = texelFetch(transformMatrix, gl_InstanceID * 4 + 2);
	vec4 col4 = texelFetch(transformMatrix, gl_InstanceID * 4 + 3);
	mat4 transform = mat4(col1, col2, col3, col4);
	
	//normalCoord = mat3(transform) * vec3(0, 1, 0);   //matrix normal * vertex normal 
	normalCoord = mat3(transform) * Normal;
	eyeCoord = vec3(transform * vec4(Position, 1));

	gl_Position = projectionMatrix * viewMatrix * transform * vec4(Position, 1.0);
}