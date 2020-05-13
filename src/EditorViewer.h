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

		API_INTERFACE virtual void zoom(sp_float scale);

		API_INTERFACE virtual void moveForward(sp_float distance);

		API_INTERFACE virtual void moveBackward(sp_float distance);

		API_INTERFACE virtual void moveLeft(sp_float distance);

		API_INTERFACE virtual void moveRight(sp_float distance);

		API_INTERFACE virtual void lookAtHorizontal(sp_float angleInRadians);

		API_INTERFACE virtual void lookAtVertical(sp_float angleInRadians);

		API_INTERFACE virtual void rotateX(sp_float angle);

		API_INTERFACE virtual void rotateY(sp_float angle);

		API_INTERFACE virtual void rotateZ(sp_float angle);

		API_INTERFACE void onKeyboardEvent(SpKeyboardEvent* evt) override;

		API_INTERFACE void onMouseEvent(SpMouseEvent* evt) override;

	};
}

#endif // VIEWER_HEADER