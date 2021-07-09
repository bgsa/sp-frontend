#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) uniform mat4          projection;
layout(location = 1) uniform mat4          view;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec4 Color;

out vec4 colorFragment;

void main()
{
	gl_Position = projection * view * vec4(Position, 1.0);
	colorFragment = Color;
}