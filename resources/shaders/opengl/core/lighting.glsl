#define SP_LIGHT_ROW_LENGTH (4)

#define SP_LIGHT_SOURCE_TYPE_AMBIENT  (1)
#define SP_LIGHT_SOURCE_TYPE_DIFFUSE  (2)
#define SP_LIGHT_SOURCE_TYPE_SPECULAR (3)

#define SP_LIGHT_SOURCE_SPOTLIGHT_CONE     (1)
#define SP_LIGHT_SOURCE_SPOTLIGHT_CILINDER (2)

struct SpLightSource
{
	int   type;
	bool  isEnabled;    /* light is turned on or off             */
	bool  isStatic;     /* light position static or dynamic      */
	float factor;       /* intensity of light (distance reached) */
	vec3  position;
	float spotlightAngle;
	vec3  direction;
	int   spotlightType;
	vec3  color;
};

void LightLoad(int lightIndex, out SpLightSource light)
{
	int textureIndex  = (lightIndex * SP_LIGHT_ROW_LENGTH);
	vec4 tex = texelFetch(lights, textureIndex);

	light.type      = floatBitsToInt(tex.x);
	light.isEnabled = bool(floatBitsToInt(tex.y));
	light.isStatic  = bool(floatBitsToInt(tex.z));
	light.factor    = tex.w;

	tex = texelFetch(lights, textureIndex + 1);
	light.position = tex.xyz;
	light.spotlightAngle = tex.w;
	
	tex = texelFetch(lights, textureIndex + 2);
	light.direction = tex.xyz;
	light.spotlightType = int(tex.w);
	
	light.color = texelFetch(lights, textureIndex + 3).xyz;
}