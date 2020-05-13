#include "EditorViewer.h"

namespace NAMESPACE_FRONTEND
{

	void EditorViewer::init(SpFrame* frame)
	{
		this->frame = frame;

		Vec3f cameraPosition = { 0.0f, 12.0f, -17.0f };
		Vec3f cameraTarget = { 0.0f, 0.0f, 0.0f };
		initProjectionPerspective(cameraPosition, cameraTarget, frame->aspectRatio());

		SpEventDispatcher::instance()->addKeyboardListener(this);
		SpEventDispatcher::instance()->addMouseListener(this);
	}

	void EditorViewer::lookAtHorizontal(sp_float angle)
	{
		Vec3f direction = target - position;

		target = Vec3f(
			position[0] + direction[0] * cos(angle) + direction[2] * sin(angle),
			target[1],
			position[2] + direction[2] * cos(angle) + (position[0] - target[0]) * sin(angle)
		);

		updateViewMatrix();
	}

	void EditorViewer::lookAtVertical(sp_float angle)
	{
		Vec3f direction = target - position;
		angle *= -1.0f;

		target = Vec3f(
			target[0],
			position[1] + direction[1] * cos(angle) + (position[2] - target[2]) * sin(angle),
			position[2] + direction[1] * sin(angle) + direction[2] * cos(angle)
		);

		updateViewMatrix();
	}

	void EditorViewer::zoom(sp_float scale)
	{
		sp_float newFieldOfView = fieldOfView + (fieldOfView * scale);

		if (newFieldOfView <= SP_MIN_FIELD_OF_VIEW || newFieldOfView >= SP_MAX_FIELD_OF_VIEW)
			return;

		fieldOfView = newFieldOfView;
		setProjectionPerspective(fieldOfView, aspectRatio, nearFrustum, farFrustum);
	}

	void EditorViewer::moveForward(sp_float distance)
	{
		Vec3f directionToMove = (position - target).normalize();
		directionToMove *= distance;

		position -= directionToMove;
		target -= directionToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveBackward(sp_float distance)
	{
		Vec3f directionToMove = (position - target).normalize();
		directionToMove *= distance;

		position += directionToMove;
		target += directionToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveLeft(sp_float distance)
	{
		Vec3f directionToMove = _up.cross(_direction).normalize();

		Vec3f distanceToMove = directionToMove * distance;

		position -= distanceToMove;
		target -= distanceToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveRight(sp_float distance)
	{
		Vec3f directionToMove = _up.cross(_direction).normalize();
		Vec3f distanceToMove = directionToMove * distance;

		position += distanceToMove;
		target += distanceToMove;

		updateViewMatrix();
	}

	void EditorViewer::rotateX(sp_float angle)
	{
		angle *= velocity;
		sp_float z = position.z;

		position = Quat::createRotate(angle, _right).rotate(position);

		if (sign(position.z) != sign(z))
			_up = -_up;

		updateViewMatrix();
	}

	void EditorViewer::rotateY(sp_float angle)
	{
		angle *= velocity;
		position = Quat::createRotationAxisY(angle).rotate(position);
		updateViewMatrix();
	}

	void EditorViewer::rotateZ(sp_float angle)
	{
		angle *= velocity;
		position = Quat::createRotate(angle, _direction).rotate(position);
		updateViewMatrix();
	}

	void EditorViewer::onKeyboardEvent(SpKeyboardEvent* evt)
	{
		if (!frame->isFocused())
			return;

		sp_int key = evt->key;
		Log::info(StringHelper::toString(key));
	}

	void EditorViewer::onMouseEvent(SpMouseEvent* evt)
	{
		if (!frame->isFocused() || !frame->isMouseHovered())
			return;

		switch (evt->type)
		{
		case SP_MOUSE_EVENT_MOVED:
			if (evt->mouse->isMiddleButtonPressed())
			{
				rotateY(degreesToRadians(evt->state.previousX - evt->state.x));
				rotateX(degreesToRadians(evt->state.previousY - evt->state.y));
			}

			break;

		default:
			break;
		}
	}

}