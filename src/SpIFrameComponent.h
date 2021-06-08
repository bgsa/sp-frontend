#ifndef SP_I_FRAME_COMPONENT_HEADER
#define SP_I_FRAME_COMPONENT_HEADER

#include "SpectrumFronend.h"
#include "SpSize.h"

namespace NAMESPACE_FRONTEND
{
	class SpIFrameComponent
	{
	private:
		sp_bool _visible;
		sp_int _width, _height;

	public:

		API_INTERFACE inline SpIFrameComponent()
		{
			_visible = false;
		}

		API_INTERFACE virtual void render() = 0;

		API_INTERFACE inline SpSize<sp_int> size() const noexcept
		{
			return { _width, _height };
		}

		API_INTERFACE inline sp_int width() const noexcept
		{
			return _width;
		}

		API_INTERFACE inline sp_int height() const noexcept
		{
			return _height;
		}

		API_INTERFACE inline void resize(sp_int width, sp_int height) noexcept
		{
			this->_width = width;
			this->_height = height;
		}

		API_INTERFACE inline sp_bool isVisible() const noexcept
		{
			return _visible;
		}

		API_INTERFACE inline void show() noexcept
		{
			_visible = true;
		}
		API_INTERFACE inline void hide() noexcept
		{
			_visible = false;
		}

	};
}

#endif // SP_I_FRAME_COMPONENT_HEADER