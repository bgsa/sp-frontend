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
		sp_bool _isFirstLoad = false;

	protected:
		IRendererManager* renderer;

	public:

		API_INTERFACE inline sp_bool isFirstLoad() const noexcept
		{
			return _isFirstLoad;
		}
		
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

		API_INTERFACE inline virtual void show() noexcept
		{
			_visible = true;
			_isFirstLoad = true;
		}
		API_INTERFACE inline virtual void hide() noexcept
		{
			_visible = false;
		}

		API_INTERFACE inline void resize(sp_int width, sp_int height) noexcept
		{
			this->_width = width;
			this->_height = height;
		}

		/// <summary>
		/// Align the window given the rate
		/// Default value is (0.5) center
		/// </summary>
		/// <param name="rate">Range from 0.0 to 1.0.</param>
		/// <returns>void</returns>
		API_INTERFACE inline void horizontalAlign(const sp_float rate = HALF_FLOAT) noexcept
		{
			ImGui::SetWindowPos(ImVec2((window()->state()->width * rate) - (width() >> 1), (window()->state()->height * rate) - (height() >> 1)));
		}

		API_INTERFACE virtual void init(SpWindow* window)
		{
			_visible = false;
			_isFocused = false;
			_isMouseHovered = false;
			_width = 300;
			_height = 200;
			_window = window;
			renderer = renderer;
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

		API_INTERFACE virtual void postRender()
		{
			_isFirstLoad = false;
		}

	};
}

#endif // SP_FRAME_HEADER