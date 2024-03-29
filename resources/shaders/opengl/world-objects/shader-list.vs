#version 300 es

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform uint transformOffset;

uniform samplerBuffer transformMatrix;

in vec3 Position;

out vec3 normalCoord, eyeCoord;

mat4 Quat_ToMat4(vec4 q)
{
	float v = q.x;
	q.x = q.z;
	q.z = v;
	
	float qxx = (q.x * q.x);
	float qyy = (q.y * q.y);
	float qzz = (q.z * q.z);
	float qxz = (q.x * q.z);
	float qxy = (q.x * q.y);
	float qyz = (q.y * q.z);
	float qwx = (q.w * q.x);
	float qwy = (q.w * q.y);
	float qwz = (q.w * q.z);

	return 
		mat4(
			1.0 - 2.0 * (qyy +  qzz),  2.0 * (qxy - qwz),        2.0 * (qxz + qwy),        0.0,
			2.0 * (qxy + qwz),         1.0 - 2.0 * (qxx +  qzz), 2.0 * (qyz - qwx),        0.0,
			2.0 * (qxz - qwy),         2.0 * (qyz + qwx),        1.0 - 2.0 * (qxx +  qyy), 0.0,
			0.0,                       0.0,                      0.0,                      1.0
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
	return 
		Mat4_Translate(position)
		* Quat_ToMat4(orientation)
		* Mat4_Scale(scale);
}

mat4 buildTransformationMatrix()
{
	vec3 position;
	vec3 scaleVec;
	vec4 orientation;

	orientation  = texelFetch(transformMatrix, 0);
	vec4 positionTemp = texelFetch(transformMatrix, 1);
	vec4 scaleTemp    = texelFetch(transformMatrix, 2);
	
	orientation = vec4(orientation.x, orientation.y, orientation.z, orientation.w);
	position = positionTemp.xyz;
	scaleVec = vec3(positionTemp.w, scaleTemp.x, scaleTemp.y);

	//orientation = vec4(1.0, 0.0, 0.0, 0.0);
	//position = vec3(0.0, 0.0, 0.0);
	//scaleVec = vec3(1.0, 1.0, 1.0);

	return SpTransform_ToMat4(position, scaleVec, orientation);
}

void main()
{
	mat4 transform = buildTransformationMatrix();

	normalCoord = mat3(transform) * vec3(0, -1, 0);   //matrix normal * vertex normal 	
	eyeCoord = vec3(transform * vec4(Position, 1));

	gl_Position = projectionMatrix * viewMatrix * transform * vec4(Position, 1.0);
}