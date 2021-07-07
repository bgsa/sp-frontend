#ifndef WORLD_OBJECTS_LIST_HEADER
#define WORLD_OBJECTS_LIST_HEADER

#include "SpectrumRendering.h"
#include "SpMesh.h"
#include "GraphicObject3DList.h"
#include "IGraphicObjectRenderer.h"
#include "OpenGLBuffer.h"
#include "SpShaderOpenGL.h"
#include "SpPhysicObjectList.h"
#include "SpLightManager.h"

namespace NAMESPACE_RENDERING
{
	class WorldObjectList
		: public GraphicObject3DList
		, public SpPhysicObjectList
	{
	private:
		SpShaderOpenGL* shader;

		sp_int lightEnvironmentLocation;
		sp_int lightPositionLocation;
		sp_int lightColorLocation;
		sp_int shininessFactorLocation;
		sp_int transformOffsetLocation;
		
		sp_uint vertexesLength;
		sp_uint indexesLength;
		sp_uint facesLength;
		sp_uint* temp;

		void initTerrain(const sp_uint rows, const sp_uint cols, Vec3* vertexes, sp_uint* indexes, const sp_float squareSize = ONE_FLOAT, const Vec3& translation = Vec3Zeros)
		{
			sp_uint vertexIndex = ZERO_UINT;
			for (sp_uint row = 0; row != rows + 1u; row++)
				for (sp_uint col = 0; col != cols + 1u; col++)
					vertexes[vertexIndex++] = Vec3(col * squareSize, ZERO_FLOAT, row * squareSize) + translation;

			_buffer = sp_mem_new(OpenGLBuffer)(VEC3_SIZE * vertexesLength, vertexes);

			sp_uint index = ZERO_UINT;
			for (sp_uint row = 0; row < rows; row++)
				for (sp_uint col = 0; col < cols; col++)
				{
					indexes[index++] = row * (cols + 1u) + col;
					indexes[index++] = row * (cols + 1u) + col + 1u;
					indexes[index++] = (row + 1u) * (cols + 1u) + col;

					indexes[index++] = (row + 1u) * (cols + 1u) + col;
					indexes[index++] = row * (cols + 1u) + col + 1u;
					indexes[index++] = (row + 1u) * (cols + 1u) + col + 1u;
				}

			_indexesBuffer = sp_mem_new(OpenGLBuffer)(index * sizeof(sp_float), indexes, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		}

		void buildMesh()
		{
			const sp_uint rows = 50u, cols = 50u, squareSize = 2u;

			vertexesLength = (rows + 1u) * (cols + 1u);
			facesLength = multiplyBy2(rows * cols);
			indexesLength = multiplyBy3(facesLength);

			Vec3* vertexes = ALLOC_NEW_ARRAY(Vec3, vertexesLength);
			sp_uint* indexes = ALLOC_NEW_ARRAY(sp_uint, indexesLength);
			
			const Vec3 translation = Vec3(rows * squareSize * -0.5f, 0.0f, cols * squareSize * -0.5f);
			initTerrain(rows, cols, vertexes, indexes, squareSize, translation);

			SpPoolMemoryAllocator::main()->enableMemoryAlignment();

			SpMesh* mesh = sp_mem_new(SpMesh)();
			mesh->vertexesMesh = sp_mem_new(SpArray<SpVertexMesh*>)(vertexesLength);
			mesh->faces = sp_mem_new(SpArray<SpFaceMesh*>)(facesLength);

			for (sp_uint i = 0; i < vertexesLength; i++)
				mesh->vertexesMesh->add(sp_mem_new(SpVertexMesh)(mesh, i, vertexes[i]));

			for (sp_uint i = 0; i < facesLength; i++)
			{
				sp_uint faceIndex = 3u * i;

				mesh->faces->add(sp_mem_new(SpFaceMesh)(mesh, i, 
					indexes[faceIndex], 
					indexes[faceIndex + 1u], 
					indexes[faceIndex + 2u])
				);
			}
			mesh->init();

			SpWorldManagerInstance->current()->mesh(physicIndex, mesh);

			SpPoolMemoryAllocator::main()->disableMemoryAlignment();

			ALLOC_RELEASE(vertexes);
		}

	public:

		API_INTERFACE WorldObjectList(const sp_uint length)
			: SpPhysicObjectList::SpPhysicObjectList(length)
		{
			SpRigidBody3D* physicProperty = rigidBody3D(0u);
			physicProperty->mass(ZERO_FLOAT);
			physicProperty->inertialTensor(Mat3Zeros);
		}

		API_INTERFACE void translate(const sp_uint index, const Vec3& translation) override
		{
			transforms(index)->translate(translation);
		}

		API_INTERFACE void scale(const sp_uint index, const Vec3& factors) override
		{
			transforms(index)->scale(factors);
		}

		API_INTERFACE void init() override
		{
			buildMesh();

			shader = sp_mem_new(SpShaderOpenGL)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources/shaders/opengl/world-objects/shader-list.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources/shaders/opengl/world-objects/shader-list.fs")
				->link();

			projectionMatrixLocation = shader->uniform("projectionMatrix");
			viewMatrixLocation = shader->uniform("viewMatrix");
			transformOffsetLocation = shader->uniform("transformOffset");

			lightColorLocation = shader->uniform("LightColor");
			lightPositionLocation = shader->uniform("LightPosition");
			lightEnvironmentLocation = shader->uniform("EnvironmentLightColor");
			shininessFactorLocation = shader->uniform("ShininessFactor");

			positionAttribute = shader->attribute("Position");
		}

		API_INTERFACE void render(const SpRenderData& renderData) override
		{
			shader->enable();
			shader->setUniform(projectionMatrixLocation, renderData.projectionMatrix);
			shader->setUniform(viewMatrixLocation, renderData.viewMatrix);
			shader->setUniform3<sp_float>(lightEnvironmentLocation, SpLightManager::instance()->environmentLight);
			shader->setUniform3<sp_float>(lightPositionLocation, SpLightManager::instance()->lights(0u)->position());
			shader->setUniform3<sp_float>(lightColorLocation, SpLightManager::instance()->lights(0u)->color());
			shader->setUniform(shininessFactorLocation, 1000.0f);
			shader->setUniform(transformOffsetLocation, physicIndex);

			_buffer->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribute);

			SpWorldManagerInstance->current()->transformsGPU()->use();
			_indexesBuffer->use();

			glDrawElements(GL_TRIANGLES, indexesLength, GL_UNSIGNED_INT, NULL);
			//glDrawElementsInstanced(GL_TRIANGLES, indexesLength, GL_UNSIGNED_INT, NULL, 1u);

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
				sp_mem_delete(shader, SpShaderOpenGL);
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