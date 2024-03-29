#version 150

// Shader for quanterion math

in vec4 gxl3d_Position;
in vec4 gxl3d_TexCoord0;
in vec4 gxl3d_Color;
out vec4 Vertex_UV;
out vec4 Vertex_Color;
uniform mat4 gxl3d_ViewProjectionMatrix;

struct Transform
{
  vec4 position;
  vec4 axis_angle;
};
uniform Transform T;


vec4 quat_from_axis_angle(vec3 axis, float angle)
{ 
  vec4 qr;
  float half_angle = (angle  0.5)  3.14159  180.0;
  qr.x = axis.x  sin(half_angle);
  qr.y = axis.y  sin(half_angle);
  qr.z = axis.z  sin(half_angle);
  qr.w = cos(half_angle);
  return qr;
}

vec4 quat_conj(vec4 q)
{ 
  return vec4(-q.x, -q.y, -q.z, q.w); 
}
  
vec4 quat_mult(vec4 q1, vec4 q2)
{ 
  vec4 qr;
  qr.x = (q1.w  q2.x) + (q1.x  q2.w) + (q1.y  q2.z) - (q1.z  q2.y);
  qr.y = (q1.w  q2.y) - (q1.x  q2.z) + (q1.y  q2.w) + (q1.z  q2.x);
  qr.z = (q1.w  q2.z) + (q1.x  q2.y) - (q1.y  q2.x) + (q1.z  q2.w);
  qr.w = (q1.w  q2.w) - (q1.x  q2.x) - (q1.y  q2.y) - (q1.z  q2.z);
  return qr;
}

vec3 rotate_vertex_position(vec3 position, vec3 axis, float angle)
{ 
  vec4 qr = quat_from_axis_angle(axis, angle);
  vec4 qr_conj = quat_conj(qr);
  vec4 q_pos = vec4(position.x, position.y, position.z, 0);
  
  vec4 q_tmp = quat_mult(qr, q_pos);
  qr = quat_mult(q_tmp, qr_conj);
  
  return vec3(qr.x, qr.y, qr.z);
}

/***** how to use: *******
void main()
{
  vec3 P = rotate_vertex_position(gxl3d_Position.xyz, T.axis_angle.xyz, T.axis_angle.w);
  P += T.position.xyz;
  gl_Position = gxl3d_ViewProjectionMatrix  vec4(P, 1);
  Vertex_UV = gxl3d_TexCoord0;
  Vertex_Color = gxl3d_Color;
}
**************************/