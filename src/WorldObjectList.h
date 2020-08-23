#ifndef WORLD_OBJECTS_LIST_HEADER
#define WORLD_OBJECTS_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3DList.h"
#include "DOP18.h"
#include "IGraphicObjectRenderer.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "SpPhysicObjectList.h"
#include "SpLightManager.h"

namespace NAMESPACE_RENDERING
{
	class WorldObjectList
		: public GraphicObject3DList
		, public SpPhysicObjectList
	{
	private:
		OpenGLShader* shader;

		sp_int lightPositionLocation;
		sp_int lightColorLocation;
		sp_int shininessFactorLocation;
		sp_int transformOffsetLocation;
		
		const sp_uint vertexIndexes[4] = { 3u, 2u, 1u, 0u, };

		const sp_float vertexes[12] = {
			-0.5f, 0.0f, -0.5f,
			0.5f, 0.0f, -0.5f,
			0.5f, 0.0f, 0.5f,
			-0.5f, 0.0f, 0.5f
		};

		void initIndexBuffer()
		{
			_indexesBuffer = sp_mem_new(OpenGLBuffer)(sizeof(vertexIndexes), vertexIndexes, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		}

		void initVertexBuffer()
		{
			_buffer = sp_mem_new(OpenGLBuffer)(sizeof(vertexes), vertexes);
		}

		void initBuffers()
		{
			initVertexBuffer();
			initIndexBuffer();
		}

		void buildMesh()
		{
			PoolMemoryAllocator::main()->enableMemoryAlignment();

			SpMesh* mesh = sp_mem_new(SpMesh)();

			mesh->vertexes = sp_mem_new(SpArray<Vec3>)(4);
			for (sp_uint i = 0; i < 4; i++)
				mesh->vertexes->add(Vec3(vertexes[3 * i], vertexes[3 * i + 1], vertexes[3 * i + 2]));

			mesh->facesIndexes = sp_mem_new(SpArray<SpPoint3<sp_uint>>)(2);
			mesh->facesIndexes->add(SpPoint3<sp_uint>(vertexIndexes[0], vertexIndexes[1], vertexIndexes[2]));
			mesh->facesIndexes->add(SpPoint3<sp_uint>(vertexIndexes[2], vertexIndexes[3], vertexIndexes[0]));
			mesh->init();

			SpPhysicSimulator::instance()->mesh(physicIndex, mesh);

			PoolMemoryAllocator::main()->disableMemoryAlignment();
		}

	public:

		API_INTERFACE WorldObjectList(const sp_uint length)
			: SpPhysicObjectList::SpPhysicObjectList(length)
		{
			DOP18* bvs = (DOP18*) boundingVolumes(0u);
			bvs[0].scale({ 1.0f, 0.2f, 1.0f });

			SpPhysicProperties* physicProperty = physicProperties(0u);
		}

		API_INTERFACE void translate(const sp_uint index, const Vec3& translation) override
		{
			transforms(index)->translate(translation);
			boundingVolumes(index)->translate(translation);
		}

		API_INTERFACE void scale(const sp_uint index, const Vec3& factors) override
		{
			transforms(index)->scale(factors);
			boundingVolumes(index)->scale(factors);
		}

		API_INTERFACE void init() override
		{
			initBuffers();
			buildMesh();

			shader = sp_mem_new(OpenGLShader)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\world-objects\\shader-list.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\world-objects\\shader-list.fs")
				->link();

			projectionMatrixLocation = shader->getUniform("projectionMatrix");
			viewMatrixLocation = shader->getUniform("viewMatrix");
			transformOffsetLocation = shader->getUniform("transformOffset");

			lightColorLocation = shader->getUniform("LightColor");
			lightPositionLocation = shader->getUniform("LightPosition");
			shininessFactorLocation = shader->getUniform("ShininessFactor");

			positionAttribute = shader->getAttribute("Position");
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			shader
				->enable()
				->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
				->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix)
				->setUniform3<sp_float>(lightPositionLocation, SpLightManager::instance()->lights()->position())
				->setUniform3<sp_float>(lightColorLocation, SpLightManager::instance()->lights()->color())
				->setUniform<sp_float>(shininessFactorLocation, 1000.0f)
				->setUniform<sp_uint>(transformOffsetLocation, physicIndex);

			_buffer->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribute);

			SpPhysicSimulator::instance()->transformsGPU()->use();
			_indexesBuffer->use();

			glDrawElementsInstanced(GL_TRIANGLE_FAN, 2 * THREE_SIZE, GL_UNSIGNED_INT, NULL, length());

			shader->disable();

			glBindVertexArray(NULL);
			glBindBuffer(GL_ARRAY_BUFFER, NULL); // Unbind
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL); // Unbind
			glBindBuffer(GL_TEXTURE_BUFFER, NULL); // Unbind
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "World Objects List";
		}

		API_INTERFACE inline void dispose() override
		{
			if (_indexesBuffer != nullptr)
			{
				sp_mem_delete(_indexesBuffer, OpenGLBuffer);
				_indexesBuffer = nullptr;
			}

			if (_buffer != nullptr)
			{
				sp_mem_delete(_buffer, OpenGLBuffer);
				_buffer = nullptr;
			}

			if (shader != nullptr)
			{
				sp_mem_delete(shader, OpenGLShader);
				shader = nullptr;
			}
		}

		~WorldObjectList()
		{
			dispose();
		}
		
	};
}

#endif // WORLD_OBJECTS_LIST_HEADER