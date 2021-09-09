#define SP_CAMERA_SIZE             (24)
#define SP_CAMERA_PROJECTION_INDEX (16)
#define SP_CAMERA_VIEW_INDEX       (20)

mat4 Mat4CameraProjection(uint cameraIndex)
{
	int index  = (int(cameraIndex) * SP_CAMERA_SIZE) + SP_CAMERA_PROJECTION_INDEX;
	
	mat4 projection;
	projection[0] = texelFetch(cameras, index    );
	projection[1] = texelFetch(cameras, index + 1);
	projection[2] = texelFetch(cameras, index + 2);
	projection[3] = texelFetch(cameras, index + 3);
	
	return projection;
}

mat4 Mat4CameraView(uint cameraIndex)
{
	int index  = (int(cameraIndex) * SP_CAMERA_SIZE) + SP_CAMERA_VIEW_INDEX;
	
	mat4 view;
	view[0] = texelFetch(cameras, index    );
	view[1] = texelFetch(cameras, index + 1);
	view[2] = texelFetch(cameras, index + 2);
	view[3] = texelFetch(cameras, index + 3);
	
	return view;
}

