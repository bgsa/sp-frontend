#version 300 es
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) uniform mat4          projection;
layout(location = 1) uniform mat4          view;
layout(location = 2) uniform unsigned int  transformIndex;
layout(location = 3) uniform samplerBuffer transforms;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;


struct Quat
{
	float w, x, y, z;
};

Quat conjugate(Quat q)
{
	Quat c;
	c.w =  q.w;
	c.x = -q.x;
	c.y = -q.y;
	c.z = -q.z;
	return c;
}


mat4 Quat_ToMat4(Quat q)
{
	q = conjugate(q);

	float ww = q.w * q.w;
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;

	// invs (inverse square length) is only required if quaternion is not already normalised
	float invs = 1.0 / (xx + yy + zz + ww);

	mat4 temp = mat4(
		(xx - yy - zz + ww) * invs              , 2.0 * ((q.x * q.y) - (q.z * q.w)) * invs, 2.0 * ((q.x * q.z) + (q.y * q.w)) * invs, 0.0,
		2.0 * ((q.x * q.y) + (q.z * q.w)) * invs, (-xx + yy - zz + ww) * invs             , 2.0 * ((q.y * q.z) - (q.x * q.w)) * invs, 0.0,
		2.0 * ((q.x * q.z) - (q.y * q.w)) * invs, 2.0 * ((q.y * q.z) + (q.x * q.w)) * invs, (-xx - yy + zz + ww) * invs             , 0.0,
		0.0                                     , 0.0                                     , 0.0                                     , 1.0
	);
	
	return temp;
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

mat4 SpTransform_ToMat4(vec3 position, vec3 scale, Quat orientation)
{
	return Mat4_Translate(position)
		*  Quat_ToMat4(orientation) 
		*  Mat4_Scale(scale);
}

mat4 buildTransformationMatrix()
{
	int index  = (gl_InstanceID + int(transformIndex)) * 3;
	int temp   = (gl_InstanceID + int(transformIndex)) * 2;
	int offset = int(float(temp) / 4.0);
	bool shift  = int(index % 2) != 0; // shift two backward
	
	vec4 orientationTemp;
	vec3 position;
	vec3 scaleVec;
	Quat orientation;
	
	if (shift)
	{
		orientationTemp   = texelFetch(transforms, index - offset - 1);
		vec4 positionTemp = texelFetch(transforms, index - offset    );
		vec4 scaleTemp    = texelFetch(transforms, index - offset + 1);
		
		orientation.w = orientationTemp.z;
		orientation.x = orientationTemp.w;
		orientation.y = positionTemp.x;
		orientation.z = positionTemp.y;
		
		position    = vec3(positionTemp.z, positionTemp.w, scaleTemp.x);
		scaleVec    = vec3(scaleTemp.y   , scaleTemp.z   , scaleTemp.w);
	}
	else
	{
		orientationTemp   = texelFetch(transforms, index - offset);
		vec4 positionTemp = texelFetch(transforms, index - offset + 1);
		vec4 scaleTemp    = texelFetch(transforms, index - offset + 2);
		
		orientation.w = orientationTemp.x;
		orientation.x = orientationTemp.y;
		orientation.y = orientationTemp.z;
		orientation.z = orientationTemp.w;

		position = positionTemp.xyz;
		scaleVec = vec3(positionTemp.w, scaleTemp.x, scaleTemp.y);
	}
	
	return SpTransform_ToMat4(position, scaleVec, orientation);
}

void main()
{
	mat4 transform = buildTransformationMatrix();
	
	gl_Position = projection * view * transform * vec4(Position, 1.0);
}