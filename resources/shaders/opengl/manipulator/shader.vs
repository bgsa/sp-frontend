#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform vec3 cameraPosition;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec4 Color;

out vec3 viewPosition;
out vec3 eyePosition;
out vec4 fragmentColor;

void main()
{
	eyePosition  = vec3(view * vec4(cameraPosition, 1.0));;	
	viewPosition = vec3(view * vec4(Position, 1.0));
	
	gl_Position = projection * view * vec4(Position, 1.0);
	
	fragmentColor = Color;
}