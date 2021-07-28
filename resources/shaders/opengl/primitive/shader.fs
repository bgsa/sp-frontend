#extension GL_ARB_explicit_uniform_location: enable

precision mediump float;

layout(location = 0) uniform samplerBuffer cameras;
layout(location = 1) uniform samplerBuffer transforms;
layout(location = 2) uniform samplerBuffer lights;
layout(location = 3) uniform int  lightsLength;
layout(location = 4) uniform unsigned int  cameraIndex;
layout(location = 5) uniform unsigned int  transformIndex;

flat in  vec3 normalCoord;
     in  vec3 eyeCoord;

     out vec4 FragOutput;

#include "../core/lighting.glsl"

vec4 PhongShading()
{
	vec3 ambientLightColor  = vec3(0.0, 0.0, 0.0);
	vec3 diffuseLightColor  = vec3(0.0, 0.0, 0.0);
	vec3 specularLightColor = vec3(0.0, 0.0, 0.0);
	vec3 fragmentNormal = normalize(normalCoord);

	for(int i = 0; i < lightsLength; i++)
	{
		SpLightSource light = LightLoad(i);
		
		if (light.isEnabled)
		{		
			switch (light.type)
			{
				case SP_LIGHT_SOURCE_TYPE_AMBIENT:
				{
					ambientLightColor = light.color;
					break;
				}
				case SP_LIGHT_SOURCE_TYPE_DIFFUSE:
				{
					// compute diffuse light
					vec3 normalizeLightVec = normalize(light.position - eyeCoord);     //vector viewer
					float cosAngle = max(0.0, dot(fragmentNormal, normalizeLightVec)); // Diffuse Intensity
					
					diffuseLightColor += cosAngle * light.color;
					break;
				}
			}
		}	
	}
	
	/*   
	// compute specular light
	vec3 R = normalize(-normalizeLightVec + normalize(-eyeCoord)); //faster
	float ShininessFactor = 1000.0;
	float specularIntensity = pow(max(0.0, dot(R, -normalizeLightVec)), ShininessFactor); // Specular Intensity
	specularLightColor = specularIntensity * LightColor;
	*/
	
	return vec4(ambientLightColor + diffuseLightColor, 1.0);
}

void main()
{
	FragOutput = PhongShading();
}