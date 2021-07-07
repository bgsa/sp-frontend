mat4 Mat4Translate(vec3 position)
{
	mat4 result;
	result[0] = vec4(1.0, 0.0, 0.0, 0.0);
	result[1] = vec4(0.0, 1.0, 0.0, 0.0);
	result[2] = vec4(0.0, 0.0, 1.0, 0.0);
	result[3] = vec4(position.x, position.y, position.z, 1.0);
	
	return result;
}

mat4 Mat4Scale(vec3 scaleVector)
{
	mat4 result;
	result[0] = vec4(scaleVector.x, 0.0, 0.0, 0.0);
	result[1] = vec4(0.0, scaleVector.y, 0.0, 0.0);
	result[2] = vec4(0.0, 0.0, scaleVector.z, 0.0);
	result[3] = vec4(0.0, 0.0, 0.0          , 1.0);
	
	return result;
}
