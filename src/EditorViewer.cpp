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
		const Vec3 camPos = position();
		const Vec3 camTar = target();
		const Vec3 camFor = forward();

		target(Vec3(
			camPos.x + camFor.x * sp_cos(angle) + camFor.z * sp_sin(angle),
			camTar.y,
			camPos.z + camFor.z * sp_cos(angle) + (camPos.x - camTar.x) * sp_sin(angle)
		));
	}

	void EditorViewer::lookAtVertical(sp_float angle)
	{
		const Vec3 camPos = position();
		const Vec3 camTar = target();
		const Vec3 camFor = forward();

		angle *= Y();

		target (Vec3(
			camTar.x,
			camPos.x + camFor.y * sp_cos(angle) + (camPos.z - camTar.z) * sp_sin(angle),
			camPos.z + camFor.y * sp_sin(angle) + camFor.z * sp_cos(angle)
		));
	}

	void EditorViewer::zoom(sp_float scale)
	{
		sp_float newFieldOfView = fieldOfView() - (fieldOfView() * scale * velocity());

		if (newFieldOfView <= SP_CAMERA_MIN_FIELD_OF_VIEW || newFieldOfView >= SP_CAMERA_MAX_FIELD_OF_VIEW)
			return;

		fieldOfView(newFieldOfView);
	}

	void EditorViewer::moveForward(sp_float distance)
	{
		Vec3 directionToMove;
		normalize(forward(), directionToMove);
		
		directionToMove *= (distance * velocity());

		position(position() - directionToMove);
		target(target() - directionToMove);
	}

	void EditorViewer::moveBackward(sp_float distance)
	{
		Vec3 directionToMove;
		normalize(forward(), directionToMove);

		directionToMove *= (distance * velocity());

		position(position() + directionToMove);
		target(target() + directionToMove);
	}

	void EditorViewer::moveLeft(sp_float distance)
	{
		Vec3 directionToMove;
		cross(up(), forward(), directionToMove);
		normalize(directionToMove);

		Vec3 distanceToMove = directionToMove * distance * velocity();

		position(position() + directionToMove);
		target(target() + directionToMove);
	}

	void EditorViewer::moveRight(sp_float distance)
	{
		Vec3 directionToMove;
		cross(up(), forward(), directionToMove);
		normalize(directionToMove);
		
		Vec3 distanceToMove = directionToMove * distance * velocity();

		position(position() - directionToMove);
		target(target() - directionToMove);
	}

	void EditorViewer::rotateX(sp_float angle)
	{
		angle *= velocity();
		const sp_float z = position().z;

		Vec3 n;
		normalize(right(), n);

		Vec3 newPosition;
		rotate(Quat::createRotate(angle, n), position(), newPosition);
		position(newPosition);

		if (sign(position().z) != sign(z))
			up(-up());
	}

	void EditorViewer::rotateY(sp_float angle)
	{
		angle *= velocity();

		Vec3 newPosition;
		rotate(Quat::createRotationAxisY(angle), position(), newPosition);

		position(newPosition);
	}

	void EditorViewer::rotateZ(sp_float angle)
	{
		angle *= velocity();

		Vec3 newPosition;
		rotate(Quat::createRotate(angle, forward()), position(), newPosition);

		position(newPosition);
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