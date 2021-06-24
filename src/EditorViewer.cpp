#include "EditorViewer.h"

namespace NAMESPACE_FRONTEND
{

	void EditorViewer::init(SpFrame* frame)
	{
		this->frame = frame;

		Vec3 cameraPosition = { 0.0f, 50.0f, 50.0f };
		Vec3 cameraTarget = { 0.0f, 0.0f, 0.0f };
		SpCamera::initProjectionPerspective(cameraPosition, cameraTarget, frame->aspectRatio());

		SpEventDispatcher::instance()->addKeyboardListener(this);
		SpEventDispatcher::instance()->addMouseListener(this);
	}

	void EditorViewer::lookAtHorizontal(sp_float angle)
	{
		target = Vec3(
			position[0] + _forward[0] * sp_cos(angle) + _forward[2] * sp_sin(angle),
			target[1],
			position[2] + _forward[2] * sp_cos(angle) + (position[0] - target[0]) * sp_sin(angle)
		);

		updateViewMatrix();
	}

	void EditorViewer::lookAtVertical(sp_float angle)
	{
		angle *= _invertY;

		target = Vec3(
			target[0],
			position[1] + _forward[1] * sp_cos(angle) + (position[2] - target[2]) * sp_sin(angle),
			position[2] + _forward[1] * sp_sin(angle) + _forward[2] * sp_cos(angle)
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
		Vec3 directionToMove;
		normalize(_forward, directionToMove);
		
		directionToMove *= (distance * _velocity);

		position -= directionToMove;
		target -= directionToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveBackward(sp_float distance)
	{
		Vec3 directionToMove;
		normalize(_forward, directionToMove);

		directionToMove *= (distance * _velocity);

		position += directionToMove;
		target += directionToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveLeft(sp_float distance)
	{
		Vec3 directionToMove;
		cross(_up, _forward, directionToMove);
		normalize(directionToMove);

		Vec3 distanceToMove = directionToMove * distance * _velocity;

		position += distanceToMove;
		target += distanceToMove;

		updateViewMatrix();
	}

	void EditorViewer::moveRight(sp_float distance)
	{
		Vec3 directionToMove;
		cross(_up, _forward, directionToMove);
		normalize(directionToMove);
		
		Vec3 distanceToMove = directionToMove * distance * _velocity;

		position -= distanceToMove;
		target -= distanceToMove;

		updateViewMatrix();
	}

	void EditorViewer::rotateX(sp_float angle)
	{
		angle *= _velocity;
		const sp_float z = position.z;

		Vec3 n;
		normalize(_right, n);

		rotate(Quat::createRotate(angle, n), position, position);

		if (sign(position.z) != sign(z))
			_up = -_up;

		updateViewMatrix();
	}

	void EditorViewer::rotateY(sp_float angle)
	{
		angle *= _velocity;		
		rotate(Quat::createRotationAxisY(angle), position, position);

		updateViewMatrix();
	}

	void EditorViewer::rotateZ(sp_float angle)
	{
		angle *= _velocity;
		rotate(Quat::createRotate(angle, _forward), position, position);

		updateViewMatrix();
	}

	sp_bool controlPressed = false;
	void EditorViewer::onKeyboardEvent(SpKeyboardEvent* evt)
	{
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

		case 341: // control
			controlPressed = evt->type == 1;
			break;

		default:
			break;
		}
	}

	void EditorViewer::onMouseEvent(SpMouseEvent* evt)
	{
		if (!frame->isFocused() || !frame->isMouseHovered())
			return;

		switch (evt->type)
		{
		case SP_MOUSE_EVENT_MOVED:
			//if (evt->mouse->isMiddleButtonPressed())
			if (controlPressed)
			{
				rotateY(radians((sp_float)evt->state.previousX - evt->state.x));
				rotateX(radians((sp_float)evt->state.previousY - evt->state.y));
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