#define SP_MATERIAL_ROW_LENGTH (4)

struct SpBaseMaterial
{
	vec4  color;
	float shininessFactor;
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
};

void MaterialLoad(uint materialIndex, out SpBaseMaterial material)
{
	int  index  = (instanceID + int(materialIndex)) * SP_MATERIAL_ROW_LENGTH;
	bool isEven  = (materialIndex % 2u) == 0u;

	if (isEven)
	{
		int offset = int(materialIndex >> 1u);
		int textureIndex = index - offset;
		
		vec4 tex1 = texelFetch(baseMaterialsBuffer, textureIndex    );
		vec4 tex2 = texelFetch(baseMaterialsBuffer, textureIndex + 1);
		vec4 tex3 = texelFetch(baseMaterialsBuffer, textureIndex + 2);
		vec4 tex4 = texelFetch(baseMaterialsBuffer, textureIndex + 3);

		material.color = tex1;
		material.shininessFactor = tex2.x;
		material.ambient  = tex2.yzw;
		material.diffuse  = tex3.xyz;
		material.specular = vec3(tex3.w, tex4.x, tex4.y);
	}
	else
	{
		int offset = int(materialIndex >> 1u) + 1;
		int textureIndex = index - offset;
		
		vec4 tex1 = texelFetch(baseMaterialsBuffer, textureIndex    );
		vec4 tex2 = texelFetch(baseMaterialsBuffer, textureIndex + 1);
		vec4 tex3 = texelFetch(baseMaterialsBuffer, textureIndex + 2);
		vec4 tex4 = texelFetch(baseMaterialsBuffer, textureIndex + 3);

		material.color = vec4(tex1.z, tex1.w, tex2.x, tex2.y);
		material.shininessFactor = tex2.z;
		material.ambient  = vec3(tex2.w, tex3.x, tex3.y);
		material.diffuse  = vec3(tex3.z, tex3.w, tex4.x);
		material.specular = vec3(tex4.y, tex4.z, tex4.w);
	}
}