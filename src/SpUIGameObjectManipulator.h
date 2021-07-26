#ifndef SP_UI_GAME_OBJECT_MANIPULATOR_HEADER
#define SP_UI_GAME_OBJECT_MANIPULATOR_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include "SpShader.h"
#include "SpGame.h"
#include "SpViewport.h"
#include "AABB.h"

namespace NAMESPACE_FRONTEND
{
#define SP_MANIPULATOR_INDEX_LENGTH (3 * 54)
#define SP_MANIPULATOR_ATTRIBUTES_STRIDE (sizeof(sp_float) * 7)

	class SpUIGameObjectManipulator
	{
	private:
		SpShader* manipulatorShader;
		SpGpuBuffer* vertexesBuffer;
		SpGpuBuffer* indexesBuffer;

		Vec3 position;
		sp_float thickness;
		sp_float distanceCameraToObject;
		sp_float manipulatorSize;

	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpUIGameObjectManipulator()
		{
			manipulatorShader = nullptr;
		}

		/// <summary>
		/// Get the x axis AABB
		/// </summary>
		/// <returns>AABB</returns>
		API_INTERFACE inline AABB xAxisAABB() const
		{
			return AABB(
				Vec3(position.x, position.y - thickness, position.z - thickness),
				Vec3(position.x + manipulatorSize, position.y + thickness, position.z + thickness)
			);
		}

		/// <summary>
		/// Get the y axis AABB
		/// </summary>
		/// <returns>AABB</returns>
		API_INTERFACE inline AABB yAxisAABB() const
		{
			return AABB(
				Vec3(position.x - thickness, position.y, position.z - thickness),
				Vec3(position.x + thickness, position.y + manipulatorSize, position.z + thickness)
			);
		}

		/// <summary>
		/// Get the z axis AABB
		/// </summary>
		/// <returns>AABB</returns>
		API_INTERFACE inline AABB zAxisAABB() const
		{
			return AABB(
				Vec3(position.x - thickness, position.y - thickness, position.z),
				Vec3(position.x + thickness, position.y + thickness, position.z + manipulatorSize)
			);
		}

		API_INTERFACE inline void init(SpShader* shader)
		{
			if (manipulatorShader != nullptr)
				return;

			const sp_uint cubeIndexes[SP_MANIPULATOR_INDEX_LENGTH] = {
				// X axis
				0, 1, 2, // front face
				2, 3, 0, // front face
				4, 7, 6, // back face
				6, 5, 4, // back face
				0, 3, 7, // left face
				7, 4, 0, // left face
				1, 5, 6, // right face
				6, 2, 1, // right face
				3, 2, 6, // top face
				6, 7, 3, // top face
				1, 0, 4, // bottom face
				4, 5, 1, // bottom face

				// X arrow
				8, 9, 10,
				10, 11, 8,
				9, 8, 12,
				10, 9, 12,
				10, 11, 12,
				8, 11, 12,

				// Y axis
				13, 14, 15, // bottom face
				15, 16, 13, // bottom face
				17, 18, 19, // top face
				19, 20, 17, // top face
				14, 15, 19, // front face
				19, 18, 14, // front face
				13, 16, 20, // back face
				20, 17, 13, // back face
				13, 14, 18, // left face
				18, 17, 13, // left face
				15, 16, 20, // right face
				20, 19, 15, // right face

				// Y arrow
				21, 22, 23,
				23, 24, 21,
				21, 22, 25,
				22, 23, 25,
				23, 24, 25,
				24, 21, 25,

				// Z axis
				26, 27, 28, // bottom face
				28, 29, 26, // bottom face
				30, 31, 32, // top face
				32, 33, 30, // top face
				27, 28, 32, // front face
				32, 31, 27, // front face
				26, 29, 33, // back face
				33, 30, 26, // back face
				26, 27, 31, // left face
				31, 30, 26, // left face
				28, 29, 33, // right face
				33, 32, 28, // right face

				// Z arrow
				34, 35, 36,
				36, 37, 34,
				34, 35, 38,
				35, 36, 38,
				36, 37, 38,
				37, 34, 38
			};

			manipulatorShader = shader;

			SpRenderingAPI* api = SpGameInstance->renderingAPI();
			vertexesBuffer = api->createArrayBuffer();

			const sp_uint typeFloatId = api->typeFloatId();
			const sp_uint typeUintId = api->typeUIntId();

			manipulatorShader
				->enableVertexAttribute(0, 3, typeFloatId, false, SP_MANIPULATOR_ATTRIBUTES_STRIDE, 0)
				->enableVertexAttribute(1, 4, typeFloatId, false, SP_MANIPULATOR_ATTRIBUTES_STRIDE, (void*)(sizeof(sp_float) * 3));

			vertexesBuffer->disable();

			const sp_uint usageTypeStatic = api->bufferUsageTypeStaticDraw();
			indexesBuffer = api
				->createElementArrayBuffer()
				->use()
				->updateData(sizeof(cubeIndexes), cubeIndexes, usageTypeStatic);

			indexesBuffer->disable();
		}

		API_INTERFACE inline void render(SpViewport& viewport, const Vec3 center)
		{
			this->position = center;

			SpRenderingAPI* api = SpGameInstance->renderingAPI();
			const sp_uint usageType = api->bufferUsageTypeDynamicDraw();
			const sp_uint typeFloatId = api->typeFloatId();
			const sp_uint typeUintId = api->typeUIntId();
			const sp_uint trianglesId = api->typeTriangleId();
			SpCamera* camera = viewport.scene->camerasManager()->get(viewport.scene->activeCameraIndex());
			const Vec3 cameraPosition = camera->position();

			distanceCameraToObject = distance(cameraPosition, position);
			manipulatorSize = distanceCameraToObject * 0.2f;
			thickness = manipulatorSize * 0.05f;
			const sp_float twoThickness = thickness * 2.0f;

			SpColorRGBA xAxisColor = SpColorRGBARed;
			SpColorRGBA yAxisColor = SpColorRGBAGreen;
			SpColorRGBA zAxisColor = SpColorRGBABlue;

			sp_float cubeAttributes[3 * 91] = {
				// x Axis
				position.x, position.y - thickness, position.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize, position.y - thickness, position.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize, position.y + thickness, position.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x, position.y + thickness, position.z + thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,

				position.x, position.y - thickness, position.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize, position.y - thickness, position.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize, position.y + thickness, position.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x, position.y + thickness, position.z - thickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,

				position.x + manipulatorSize, position.y - twoThickness, position.z - twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize, position.y + twoThickness, position.z - twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize, position.y + twoThickness, position.z + twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize, position.y - twoThickness, position.z + twoThickness, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,
				position.x + manipulatorSize + twoThickness, position.y, position.z, xAxisColor.red, xAxisColor.green, xAxisColor.blue, xAxisColor.alpha,

				// y Axis
				position.x - thickness, position.y, position.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x - thickness, position.y, position.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x + thickness, position.y, position.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x + thickness, position.y, position.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,

				position.x - thickness, position.y + manipulatorSize, position.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x - thickness, position.y + manipulatorSize, position.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x + thickness, position.y + manipulatorSize, position.z - thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x + thickness, position.y + manipulatorSize, position.z + thickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,

				position.x - twoThickness, position.y + manipulatorSize, position.z + twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x - twoThickness, position.y + manipulatorSize, position.z - twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x + twoThickness, position.y + manipulatorSize, position.z - twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x + twoThickness, position.y + manipulatorSize, position.z + twoThickness, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,
				position.x, position.y + manipulatorSize + twoThickness, position.z, yAxisColor.red, yAxisColor.green, yAxisColor.blue, yAxisColor.alpha,

				// z Axis
				position.x + thickness, position.y - thickness, position.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x - thickness, position.y - thickness, position.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x - thickness, position.y + thickness, position.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x + thickness, position.y + thickness, position.z, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,

				position.x + thickness, position.y - thickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x - thickness, position.y - thickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x - thickness, position.y + thickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x + thickness, position.y + thickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,

				position.x + twoThickness, position.y - twoThickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x - twoThickness, position.y - twoThickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x - twoThickness, position.y + twoThickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x + twoThickness, position.y + twoThickness, position.z + manipulatorSize, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha,
				position.x, position.y, position.z + manipulatorSize + twoThickness, zAxisColor.red, zAxisColor.green, zAxisColor.blue, zAxisColor.alpha
			};

			//api->disableDepthTest();
			viewport.framebuffer()->use();
			manipulatorShader
				->enable()
				->setUniform(0, camera->projectionMatrix())
				->setUniform(1, camera->viewMatrix())
				->setUniform(2, cameraPosition);

			vertexesBuffer
				->use()
				->updateData(sizeof(cubeAttributes), cubeAttributes, usageType);

			manipulatorShader
				->enableVertexAttribute(0, 3, typeFloatId, false, SP_MANIPULATOR_ATTRIBUTES_STRIDE, 0)
				->enableVertexAttribute(1, 4, typeFloatId, false, SP_MANIPULATOR_ATTRIBUTES_STRIDE, (void*)(sizeof(sp_float) * 3));

			indexesBuffer->use();

			manipulatorShader->drawElements(trianglesId, SP_MANIPULATOR_INDEX_LENGTH, typeUintId, 0);

			manipulatorShader->disable();

			viewport.framebuffer()->disable();

			//api->enableDepthTest();
		}

	};

#undef SP_MANIPULATOR_ATTRIBUTES_STRIDE
#undef SP_MANIPULATOR_INDEX_LENGTH
}

#endif // SP_UI_GAME_OBJECT_MANIPULATOR_HEADER