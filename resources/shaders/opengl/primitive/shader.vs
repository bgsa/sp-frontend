#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) uniform mat4          projection;
layout(location = 1) uniform mat4          view;
layout(location = 2) uniform unsigned int  transformIndex;
layout(location = 3) uniform samplerBuffer transforms;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

flat out vec3 normalCoord;
     out vec3 eyeCoord;

#include "../core/quat.glsl"
#include "../core/mat4.glsl"
#include "../core/transform.glsl"

void main()
{
	mat4 transform = Mat4Transform();
	
	normalCoord = mat3(transform) * Normal;
	eyeCoord    = vec3(transform * vec4(Position, 1));
	
	gl_Position = projection * view * transform * vec4(Position, 1.0);
}