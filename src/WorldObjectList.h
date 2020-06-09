#ifndef WORLD_OBJECTS_LIST_HEADER
#define WORLD_OBJECTS_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3DList.h"
#include "DOP18.h"
#include "IGraphicObjectRenderer.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "SpPhysicObjectList.h"

namespace NAMESPACE_RENDERING
{
	class WorldObjectList
		: public GraphicObject3DList
		, public SpPhysicObjectList
	{
	private:
		OpenGLShader* shader;

		WorldObjectList* translate(const Vec3& translation) override { return nullptr; }
		WorldObjectList* scale(const Vec3& scaleVector) override { return nullptr;  }
		
		void initIndexBuffer()
		{
			sp_uint vertexIndexes[4] = { 3u, 2u, 1u, 0u, };
			_indexesBuffer = sp_mem_new(OpenGLBuffer)(sizeof(vertexIndexes), vertexIndexes, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		}

		void initTransformBuffer()
		{
			_transformsBuffer = sp_mem_new(OpenGLTextureBuffer)();

			Mat4* transformsAsMat4 = ALLOC_NEW_ARRAY(Mat4, MAT4_LENGTH * _length);
			SpTransform* transforms = GraphicObject3DList::transforms(0u);

			for (sp_uint i = 0; i < _length; i++)
				std::memcpy(&transformsAsMat4[i], transforms[i].toMat4(), MAT4_SIZE);

			_transformsBuffer
				->use()
				->setData(MAT4_SIZE * _length, transformsAsMat4, GL_DYNAMIC_DRAW);

			ALLOC_RELEASE(transformsAsMat4);
		}

		void initVertexBuffer()
		{
			sp_float vertexes[12] = {
				-0.5f, 0.0f, -0.5f,
				0.5f, 0.0f, -0.5f,
				0.5f, 0.0f, 0.5f,
				-0.5f, 0.0f, 0.5f
			};

			_buffer = sp_mem_new(OpenGLBuffer)(sizeof(vertexes), vertexes);
		}

		void initBuffers()
		{
			initVertexBuffer();
			initIndexBuffer();
			initTransformBuffer();
		}

	public:

		API_INTERFACE WorldObjectList(const sp_uint length)
			: SpPhysicObjectList::SpPhysicObjectList(length)
		{
			GraphicObject3DList::setLength(length);

			DOP18* bvs = (DOP18*) boundingVolumes(0u);
			bvs[0].scale({ 1.0f, 0.5f, 1.0f });

			SpPhysicProperties* physicProperty = physicProperties(0u);
		}

		API_INTERFACE inline sp_uint length() const override { return _length; }

		API_INTERFACE void translate(const sp_uint index, const Vec3& translation) override
		{
			GraphicObject3DList::transforms(index)->translate(translation);
			boundingVolumes(index)->translate(translation);
		}

		API_INTERFACE void scale(const sp_uint index, const Vec3& factors) override
		{
			GraphicObject3DList::transforms(index)->scale(factors);
			boundingVolumes(index)->scale(factors);
		}

		API_INTERFACE void init() override
		{
			initBuffers();

			shader = sp_mem_new(OpenGLShader)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\world-objects\\shader-list.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\world-objects\\shader-list.fs")
				->link();

			projectionMatrixLocation = shader->getUniform("projectionMatrix");
			viewMatrixLocation = shader->getUniform("viewMatrix");
			transformMatrixLocation = shader->getUniform("transformMatrix");

			positionAttribute = shader->getAttribute("Position");
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			shader
				->enable()
				->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
				->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix);

			_buffer->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribute);

			_transformsBuffer->use();			
			_indexesBuffer->use();

			glDrawElementsInstanced(GL_TRIANGLE_FAN, 2 * THREE_SIZE, GL_UNSIGNED_INT, NULL, _length);

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