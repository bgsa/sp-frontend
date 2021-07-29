#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) uniform samplerBuffer cameras;
layout(location = 1) uniform samplerBuffer transforms;
layout(location = 2) uniform samplerBuffer lights;
layout(location = 3) uniform samplerBuffer materials;
layout(location = 4) uniform int  lightsLength;
layout(location = 5) uniform unsigned int  cameraIndex;
layout(location = 6) uniform unsigned int  transformIndex;
layout(location = 7) uniform unsigned int  renderableObjectIndex;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

flat out int  instanceID;
flat out vec3 normalCoord;
     out vec3 eyeCoord;

#include "../core/quat.glsl"
#include "../core/mat4.glsl"
#include "../core/camera.glsl"
#include "../core/transform.glsl"

void main()
{
	instanceID  = gl_InstanceID;

	mat4 projection = Mat4CameraProjection(cameraIndex);
	mat4 view       = Mat4CameraView(cameraIndex);
	mat4 transform  = Mat4Transform(transformIndex);

	normalCoord = mat3(transform) * Normal;
	eyeCoord    = vec3(transform * vec4(Position, 1));
	

	gl_Position = projection * view * transform * vec4(Position, 1.0);
}