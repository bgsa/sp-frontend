#version 300 es

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform uint transformOffset;

uniform samplerBuffer transformMatrix;

in vec3 Position;
in vec3 Normal;

out vec3 normalCoord, eyeCoord;


mat4 Quat_ToMat4(vec4 quat)
{
	float sqw = quat.w * quat.w;
	float sqx = quat.x * quat.x;
	float sqy = quat.y * quat.y;
	float sqz = quat.z * quat.z;

	float m00 = sqx - sqy - sqz + sqw; // since sqw + sqx + sqy + sqz =1
	float m11 = -sqx + sqy - sqz + sqw;
	float m22 = -sqx - sqy + sqz + sqw;

	float tmp1 = quat.x * quat.y;
	float tmp2 = quat.z * quat.w;
	float m01 = 2.0 * (tmp1 + tmp2);
	float m10 = 2.0 * (tmp1 - tmp2);

	tmp1 = quat.x * quat.z;
	tmp2 = quat.y * quat.w;
	float m02 = 2.0 * (tmp1 - tmp2);
	float m20 = 2.0 * (tmp1 + tmp2);

	tmp1 = quat.y * quat.z;
	tmp2 = quat.x * quat.w;
	float m12 = 2.0 * (tmp1 + tmp2);
	float m21 = 2.0 * (tmp1 - tmp2);

	return mat4(
		m00, m10, m20, 0.0,
		m01, m11, m21, 0.0,
		m02, m12, m22, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 Mat4_Translate(vec3 position)
{
	mat4 result;
	result[0] = vec4(1.0, 0.0, 0.0, 0.0);
	result[1] = vec4(0.0, 1.0, 0.0, 0.0);
	result[2] = vec4(0.0, 0.0, 1.0, 0.0);
	result[3] = vec4(position.x, position.y, position.z, 1.0);
	
	return result;
}

mat4 Mat4_Scale(vec3 scaleVector)
{
	mat4 result;
	result[0] = vec4(scaleVector.x, 0.0, 0.0, 0.0);
	result[1] = vec4(0.0, scaleVector.y, 0.0, 0.0);
	result[2] = vec4(0.0, 0.0, scaleVector.z, 0.0);
	result[3] = vec4(0.0, 0.0, 0.0          , 1.0);
	
	return result;
}

mat4 SpTransform_ToMat4(vec3 position, vec3 scale, vec4 orientation)
{
	return Mat4_Translate(position)
		* Quat_ToMat4(orientation)
		* Mat4_Scale(scale);
}

mat4 buildTransformationMatrix()
{
	int index  = (gl_InstanceID + int(transformOffset)) * 3;
	int temp   = (gl_InstanceID + int(transformOffset)) * 2;
	int offset = int(float(temp) / 4.0);
	bool shift  = int(index % 2) != 0; // shift two backward
	
	vec4 orientation;
	vec3 position;
	vec3 scaleVec;
	
	if (shift)
	{
		orientation       = texelFetch(transformMatrix, index - offset - 1);
		vec4 positionTemp = texelFetch(transformMatrix, index - offset    );
		vec4 scaleTemp    = texelFetch(transformMatrix, index - offset + 1);
		
		orientation = vec4(orientation.z , orientation.w , positionTemp.x, positionTemp.y);
		position    = vec3(positionTemp.z, positionTemp.w, scaleTemp.x                   );
		scaleVec    = vec3(scaleTemp.y   , scaleTemp.z   , scaleTemp.w                   );
	}
	else
	{
		orientation       = texelFetch(transformMatrix, index - offset);
		vec4 positionTemp = texelFetch(transformMatrix, index - offset + 1);
		vec4 scaleTemp    = texelFetch(transformMatrix, index - offset + 2);
		
		orientation = vec4(orientation.w, orientation.x, orientation.y, orientation.z);
		position = positionTemp.xyz;
		scaleVec = vec3(positionTemp.w, scaleTemp.x, scaleTemp.y);
	}
	
	return SpTransform_ToMat4(position, scaleVec, orientation);
}


void main()
{	
	mat4 transform = buildTransformationMatrix();
	
	//normalCoord = mat3(transform) * vec3(0, 1, 0);   //matrix normal * vertex normal 
	normalCoord = mat3(transform) * Normal;
	eyeCoord = vec3(transform * vec4(Position, 1));

	gl_Position = projectionMatrix * viewMatrix * transform * vec4(Position, 1.0);
}