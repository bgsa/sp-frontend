precision mediump float;

in  vec3 eyePosition;
in  vec3 viewPosition;
in  vec4 fragmentColor;

out vec4 FragOutput;

vec4 FlatShading(vec3 faceNormal)
{
	vec3 lightVector = normalize(eyePosition - viewPosition); // fragment position to viewer vector

	float cosAngle = max(0.0, dot(faceNormal, lightVector)); // angle face normal to viewer

	vec3 diffuse = cosAngle * fragmentColor.xyz;
	
	vec3 EnvironmentLightColor = vec3(0.3, 0.3, 0.3);
	
	return vec4(EnvironmentLightColor + diffuse, 1.0);
}

void main()
{
    vec3 xTangent = dFdx( viewPosition );
    vec3 yTangent = dFdy( viewPosition );
    vec3 faceNormal = normalize( cross( xTangent, yTangent ) ); // flat shading

	FragOutput = FlatShading(faceNormal);
}
