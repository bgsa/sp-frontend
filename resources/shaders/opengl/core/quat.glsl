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

mat4 Mat4Rotation(Quat q)
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
