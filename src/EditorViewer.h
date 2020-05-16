#ifndef VIEWER_HEADER
#define VIEWER_HEADER

#include "SpectrumFronend.h"
#include "SpKeyboardEventListener.h"
#include "SpMouseEventListener.h"
#include "SpKeyboardEvent.h"
#include "SpMouse.h"
#include "SpMouseEvent.h"
#include "SpEventDispatcher.h"
#include "Camera.h"
#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class EditorViewer
		: public Camera
		, public SpKeyboardEventListener
		, public SpMouseEventListener
	{
	private:
		SpFrame* frame;
		Camera* camera;

	public:

		API_INTERFACE void init(SpFrame* frame);

		API_INTERFACE void zoom(sp_float scale) override;

		API_INTERFACE void moveForward(sp_float distance) override;

		API_INTERFACE void moveBackward(sp_float distance) override;

		API_INTERFACE void moveLeft(sp_float distance) override;

		API_INTERFACE void moveRight(sp_float distance) override;

		API_INTERFACE void lookAtHorizontal(sp_float angleInRadians) override;

		API_INTERFACE void lookAtVertical(sp_float angleInRadians) override;

		API_INTERFACE void rotateX(sp_float angle) override;

		API_INTERFACE void rotateY(sp_float angle) override;

		API_INTERFACE void rotateZ(sp_float angle) override;

		API_INTERFACE void onKeyboardEvent(SpKeyboardEvent* evt) override;

		API_INTERFACE void onMouseEvent(SpMouseEvent* evt) override;

	};
}

#endif // VIEWER_HEADER