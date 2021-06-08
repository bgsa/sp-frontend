#ifndef SP_UI_NOTIFICATION_MANAGER_HEADER
#define SP_UI_NOTIFICATION_MANAGER_HEADER

#include "SpectrumFronend.h"
#include "NotificationFrame.h"

namespace NAMESPACE_FRONTEND
{
#define SP_UI_MAX_NOTIFICATION 10
#define SP_UI_NOTIFICATION_LIFETIME 200

	class SpUINotificationManager
	{
	private:
		NotificationFrame notifications[SP_UI_MAX_NOTIFICATION];
		sp_ushort _notificationsLength;
		sp_uint notificationIndex;

	public:

		API_INTERFACE inline SpUINotificationManager()
		{
		}

		API_INTERFACE inline void initialize()
		{
			// Init all notifications
			notificationIndex = ZERO_UINT;
			_notificationsLength = ZERO_USHORT;
			for (sp_int i = 0; i < SP_UI_MAX_NOTIFICATION; i++)
			{
				notifications[i].id = i;
				notifications[i].init();
			}
		}

		API_INTERFACE inline void render()
		{
			sp_int idx = ZERO_INT;
			for (sp_int i = 0; i < SP_UI_MAX_NOTIFICATION; i++)
				if (notifications[i].isVisible())
				{
					notifications[i].index = idx++;
					notifications[i].render();
				}
		}

		API_INTERFACE inline sp_ushort notificationsLength() const
		{
			return _notificationsLength;
		}

		API_INTERFACE inline sp_int addNotification(const sp_char* message, const NotificationType type, const sp_uint lifetime = SP_UI_NOTIFICATION_LIFETIME)
		{
			const sp_size messageLength = std::strlen(message);

			std::memcpy(notifications[notificationIndex].message, message, messageLength + 1);

			notifications[notificationIndex].lifetime = lifetime;
			notifications[notificationIndex].type = type;
			notifications[notificationIndex].show();

			if (notificationIndex + 1 == SP_UI_MAX_NOTIFICATION)
				notificationIndex = ZERO_UINT;
			else
				notificationIndex++;

			_notificationsLength++;

			return notifications[notificationIndex].id;
		}

		API_INTERFACE inline void removeNotification(const sp_int id)
		{
			notifications[id].hide();
			_notificationsLength--;
		}

		API_INTERFACE static void init();
		API_INTERFACE static void release();

	};

	extern SpUINotificationManager* SpUINotificationManagerInstance;
}

#endif // SP_UI_NOTIFICATION_MANAGER_HEADER