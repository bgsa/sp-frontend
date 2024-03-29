#version 300 es
precision mediump float;

uniform vec3  LightPosition;
uniform vec3  LightColor;
uniform vec3  EnvironmentLightColor;
uniform float ShininessFactor;

in vec3  normalCoord;
in vec3  eyeCoord;

out vec4 FragOutput;

vec4 PhongShading()
{
	vec3 normalizeNormal = normalize(normalCoord);
	vec3 normalizeLightVec = normalize(LightPosition - eyeCoord); //vector viewer

	float cosAngle = max(0.0, dot(normalizeNormal, normalizeLightVec)); // Diffuse Intensity

	vec3 R = normalize(-normalizeLightVec + normalize(-eyeCoord)); //faster

	float specularIntensity = pow(max(0.0, dot(R, -normalizeLightVec)), ShininessFactor); // Specular Intensity

	vec3 diffuse = cosAngle * LightColor;
	vec3 specular = specularIntensity * LightColor;

	return vec4(EnvironmentLightColor + diffuse + specular, 1.0);
}

void main()
{
	FragOutput = vec4(0.5, 0.5, 0.5, 1.0) * PhongShading();
	//FragOutput = vec4(0.5, 0.5, 1.0, 0.7);
}