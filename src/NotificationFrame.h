#ifndef NOTIFICATION_FRAME_HEADER
#define NOTIFICATION_FRAME_HEADER

#include "SpIFrameComponent.h"
#include "SpUIIcons.h"
#include "SpUITextComponent.h"

namespace NAMESPACE_FRONTEND
{
	enum NotificationType
	{
		OK,
		WARNING,
		VALIDATION,
		ERROR_
	};

	class NotificationFrame
		: public SpIFrameComponent
	{
	private:
		SpUITextComponent text;
		sp_char name[21];

	public:
		NotificationType type;
		sp_int id;
		sp_int index;
		sp_uint lifetime = 200;
		sp_char message[256];

		API_INTERFACE void init()
		{
			resize(400, 60);

			sp_char address[56];
			sp_uint l;
			convert((sp_uint)(sp_size)this, address, &l);
			std::memcpy(name, "Notification", 12);
			std::memcpy(&name[12], &address, 8);
			name[20] = END_OF_STRING;

			text.init();
		}

		API_INTERFACE void render() override;

	};
}

#endif // NOTIFICATION_FRAME_HEADER