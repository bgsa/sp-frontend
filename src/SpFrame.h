#ifndef SP_FRAME_HEADER
#define SP_FRAME_HEADER

#include "SpectrumFronend.h"
#include "SpWindow.h"
#include "IRendererManager.h"
#include "RendererSettings.h"
#include "RenderData.h"
#include "imgui/imgui.h"
#include "SpSize.h"

namespace NAMESPACE_FRONTEND
{
	class SpFrame
		: public Object
	{
	private:
		sp_bool _visible;
		sp_int _width;
		sp_int _height;
		SpWindow* _window;
		sp_bool _isFocused;
		sp_bool _isMouseHovered;

	protected:
		IRendererManager* renderer;

	public:
		
		API_INTERFACE inline sp_float aspectRatio() const noexcept
		{
			return (sp_float)(_width / _height);
		}

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

		API_INTERFACE inline sp_bool isVisible() const noexcept
		{
			return _visible;
		}

		API_INTERFACE inline sp_bool isFocused() const noexcept
		{
			return _isFocused;
		}

		API_INTERFACE inline sp_bool isMouseHovered() const noexcept
		{
			return _isMouseHovered;
		}

		API_INTERFACE inline void show() noexcept
		{
			this->_visible = true;
		}
		API_INTERFACE inline void hide() noexcept
		{
			this->_visible = false;
		}

		API_INTERFACE inline void resize(sp_int width, sp_int height) noexcept
		{
			this->_width = width;
			this->_height = height;
		}

		API_INTERFACE virtual void init(SpWindow* window)
		{
			this->_visible = false;
			this->_isFocused = false;
			this->_isMouseHovered = false;
			this->_width = 300;
			this->_height = 200;
			this->_window = window;
			this->renderer = renderer;
		};

		API_INTERFACE virtual void loadState()
		{
			ImVec2 frameSize = ImGui::GetWindowSize();
			_width  = (sp_int)frameSize.x;
			_height = (sp_int)frameSize.y;

			_isFocused = ImGui::IsWindowFocused();
			_isMouseHovered = ImGui::IsWindowHovered();
		}

		API_INTERFACE void setRenderer(IRendererManager* renderer)
		{
			this->renderer = renderer;
		}

		API_INTERFACE inline SpWindow* window() const
		{
			return _window;
		};

		API_INTERFACE virtual void preRender() = 0;

		API_INTERFACE virtual void render() = 0;

		API_INTERFACE virtual void renderGUI() = 0;

		API_INTERFACE virtual void postRender() = 0;

	};
}

#endif // SP_FRAME_HEADER