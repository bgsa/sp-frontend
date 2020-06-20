#include "EditorViewer.h"

namespace NAMESPACE_FRONTEND
{

	void EditorViewer::init(SpFrame* frame)
	{
		this->frame = frame;

		Vec3 cameraPosition = { 0.0f, 30.0f, 25.0f };
		Vec3 cameraTarget = { 0.0f, 0.0f, 0.0f };
		initProjectionPerspective(cameraPosition, cameraTarget, frame->aspectRatio());

		SpEventDispatcher::instance()->addKeyboardListener(this);
		SpEventDispatcher::instance()->addMouseListener(this);
	}

	void EditorViewer::lookAtHorizontal(sp_float angle)
	{
		target = Vec3(
			position[0] + _direction[0] * std::cosf(angle) + _direction[2] * std::sinf(angle),
			target[1],
			position[2] + _direction[2] * std::cosf(angle) + (position[0] - target[0]) * std::sinf(angle)
		);

		updateViewMatrix();
	}

	void EditorViewer::lookAtVertical(sp_float angle)
	{
		angle *= _invertY;

		target = Vec3(
			target[0],
			position[1] + _direction[1] * std::cosf(angle) + (position[2] - target[2]) * std::sinf(angle),
			position[2] + _direction[1] * std::sinf(angle) + _direction[2] * std::cosf(angle)
		);

		updateViewMatrix();
	}

	void EditorViewer::zoom(sp_float scale)
	{
		sp_float newFieldOfView = _fieldOfView - (_fieldOfView * scale * _velocity);

		if (newFieldOfView <= SP_MIN_FIELD_OF_VIEW || newFieldOfView >= SP_MAX_FIELD_OF_VIEW)
			return;

		_fieldOfView = newFieldOfView;
		setProjectionPerspective(_fieldOfView, aspectRatio, nearFrustum, farFrustum);
	}

	void EditorViewer::moveForward(sp_float distance)
	{
		Vec3 directionToMove = _direction.normalize();
		directionToMove *= (distance * _velocity);

		position -= directionToMove;
		target -= directionToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveBackward(sp_float distance)
	{
		Vec3 directionToMove = _direction.normalize();
		directionToMove *= (distance * _velocity);

		position += directionToMove;
		target += directionToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveLeft(sp_float distance)
	{
		Vec3 directionToMove = _up.cross(_direction).normalize();

		Vec3 distanceToMove = directionToMove * distance * _velocity;

		position += distanceToMove;
		target += distanceToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveRight(sp_float distance)
	{
		Vec3 directionToMove = _up.cross(_direction).normalize();
		Vec3 distanceToMove = directionToMove * distance * _velocity;

		position -= distanceToMove;
		target -= distanceToMove;

		updateViewMatrix();
	}

	void EditorViewer::rotateX(sp_float angle)
	{
		angle *= _velocity;
		sp_float z = position.z;

		position = Quat::createRotate(angle, _right).rotate(position);

		if (sign(position.z) != sign(z))
			_up = -_up;

		updateViewMatrix();
	}

	void EditorViewer::rotateY(sp_float angle)
	{
		angle *= _velocity;
		position = Quat::createRotationAxisY(angle).rotate(position);
		updateViewMatrix();
	}

	void EditorViewer::rotateZ(sp_float angle)
	{
		angle *= _velocity;
		position = Quat::createRotate(angle, _direction).rotate(position);
		updateViewMatrix();
	}

	void EditorViewer::onKeyboardEvent(SpKeyboardEvent* evt)
	{
		/*
		if (!frame->isFocused())
			return;

		switch (evt->key)
		{
		case 65: // left
			moveLeft(TWO_FLOAT);
			break;

		case 68: // right
			moveRight(TWO_FLOAT);
			break;

		case 87: // up
			moveForward(TWO_FLOAT);
			break;

		case 83: // down
			moveBackward(TWO_FLOAT);
			break;

		default:
			break;
		}
		*/
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

		case SP_MOUSE_EVENT_SCROLLED:
			zoom(evt->state.scrollDeltaY);
			break;

		default:
			break;
		}
	}

}