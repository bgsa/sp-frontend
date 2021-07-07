mat4 Mat4Transform(vec3 position, vec3 scale, Quat orientation)
{
	return Mat4Translate(position)
		*  Mat4Rotation(orientation) 
		*  Mat4Scale(scale);
}

mat4 Mat4Transform()
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
	
	return Mat4Transform(position, scaleVec, orientation);
}
