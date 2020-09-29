#ifndef WORLD_OBJECTS_LIST_HEADER
#define WORLD_OBJECTS_LIST_HEADER

#include "SpectrumRendering.h"
#include "SpMesh.h"
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

		sp_int lightEnvironmentLocation;
		sp_int lightPositionLocation;
		sp_int lightColorLocation;
		sp_int shininessFactorLocation;
		sp_int transformOffsetLocation;
		
		const sp_uint vertexIndexes[4] = { 0u, 1u, 2u, 3u, };

		const sp_float vertexes[12] = {
			-0.5f, 0.0f, 0.5f,
			0.5f, 0.0f, 0.5f,
			0.5f, 0.0f, -0.5f,
			-0.5f, 0.0f, -0.5f
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

			mesh->vertexesMesh = sp_mem_new(SpArray<SpVertexMesh*>)(4);
			for (sp_uint i = 0; i < 4; i++)
			{
				Vec3 v(vertexes[3 * i], vertexes[3 * i + 1], vertexes[3 * i + 2]);
				mesh->vertexesMesh->add(sp_mem_new(SpVertexMesh)(mesh, i, v));
			}

			mesh->faces = sp_mem_new(SpArray<SpFaceMesh*>)(2);
			mesh->faces->add(sp_mem_new(SpFaceMesh(mesh, 0, 0, 1, 2)));
			mesh->faces->add(sp_mem_new(SpFaceMesh(mesh, 1, 2, 3, 0)));

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
			physicProperty->mass(ZERO_FLOAT);
			physicProperty->inertialTensor(Mat3(ZERO_FLOAT));
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
			lightEnvironmentLocation = shader->getUniform("EnvironmentLightColor");
			shininessFactorLocation = shader->getUniform("ShininessFactor");

			positionAttribute = shader->getAttribute("Position");
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			shader
				->enable()
				->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
				->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix)
				->setUniform3<sp_float>(lightEnvironmentLocation, SpLightManager::instance()->environmentLight)
				->setUniform3<sp_float>(lightPositionLocation, SpLightManager::instance()->lights(0u)->position())
				->setUniform3<sp_float>(lightColorLocation, SpLightManager::instance()->lights(0u)->color())
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