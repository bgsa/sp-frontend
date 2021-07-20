#ifndef SP_I_FRAME_COMPONENT_HEADER
#define SP_I_FRAME_COMPONENT_HEADER

#include "SpectrumFronend.h"
#include "SpSize.h"
#include "SpImGui.h"

namespace NAMESPACE_FRONTEND
{
	class SpIFrameComponent
	{
	protected:
		sp_bool _visible;
		sp_bool _isFocused;
		sp_int _width, _height;
		sp_float _minWidth, _minHeight;
		ImVec2 _windowPosition;

		ImVec2 previousMousePosition;
		sp_bool wasMouseHovered;
		sp_bool wasMouseLeftButtonDown;
		sp_bool _wasMouseDownStartedThisWindow;

	public:

		void (*onMouseEnter)(SpIFrameComponent* window, const ImVec2& mousePosition);
		void (*onMouseLeave)(SpIFrameComponent* window, const ImVec2& mousePosition);
		void (*onMouseMove)(SpIFrameComponent* window, const ImVec2& previousPosition, const ImVec2& currentPosition);
		void (*onMouseLeftButtonDown)(SpIFrameComponent* window, const ImVec2& mousePosition);
		void (*onMouseLeftButtonReleased)(SpIFrameComponent* window, const ImVec2& mousePosition);
		
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpIFrameComponent()
		{
			_visible = false;
			_isFocused = false;
			_minWidth = _minHeight = ZERO_FLOAT;
			
			previousMousePosition = ImVec2Zeros;
			wasMouseHovered = false;
			wasMouseLeftButtonDown = false;
			_wasMouseDownStartedThisWindow = false;

			onMouseEnter = nullptr;
			onMouseLeave = nullptr;
			onMouseMove = nullptr;
			onMouseLeftButtonDown = nullptr;
			onMouseLeftButtonReleased = nullptr;
		}

		/// <summary>
		/// Check the cursor is hover the window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_bool isMouseHovered() const
		{
			if (isFocused())
			{
				ImGuiIO io = ImGui::GetIO();
				if (io.MousePos.x >= _windowPosition.x && io.MousePos.x <= _windowPosition.x + _width &&
					io.MousePos.y >= _windowPosition.y && io.MousePos.y <= _windowPosition.y + _height)
					return true;
			}

			return false;
		}

		/// <summary>
		/// Get the mouse position in window coordinates
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline ImVec2 screenPosition(const ImVec2 point) const
		{
			const sp_float x = sp_clamp(point.x - _windowPosition.x, 0.0f, (sp_float)_width);
			const sp_float y = sp_clamp(point.y - _windowPosition.y, 0.0f, (sp_float)_height);

			return ImVec2(x, y);
		}

		/// <summary>
		/// Check the mouse down was started in this window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_bool wasMouseDownStartedThisWindow() const
		{
			return _wasMouseDownStartedThisWindow;
		}

		/// <summary>
		/// Render this window component
		/// </summary>
		/// <returns></returns>
		API_INTERFACE virtual void render() = 0;

		/// <summary>
		/// Get the size of this window (width, height)
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpSize<sp_int> size() const noexcept
		{
			return { _width, _height };
		}

		/// <summary>
		/// Get the width of this window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_int width() const noexcept
		{
			return _width;
		}

		/// <summary>
		/// Get the height of this window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_int height() const noexcept
		{
			return _height;
		}

		/// <summary>
		/// Get the minimum width of this window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_float minWidth() const noexcept
		{
			return _minWidth;
		}

		/// <summary>
		/// Get the maximum geight of this window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_float minHeight() const noexcept
		{
			return _minHeight;
		}

		/// <summary>
		/// Check the window was resized
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_bool wasResized() const
		{
			ImVec2 currentSize = ImGui::GetWindowSize();

			return currentSize.x != _width 
				|| currentSize.y != _height;
		}

		/// <summary>
		/// Ensure the minimum size for the frame is set
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void ensureMinSize()
		{
			ImVec2 currentSize = ImGui::GetWindowSize();

			if (currentSize.x < _minWidth)
				currentSize.x = _minWidth;

			if (currentSize.y < _minHeight)
				currentSize.y = _minHeight;

			ImGui::SetWindowSize(currentSize);
		}

		API_INTERFACE inline sp_bool begin(const sp_char*title, sp_bool* opened = 0, const ImGuiWindowFlags flags= 0)
		{
			const sp_bool isOpened = ImGui::Begin(title, opened, flags);

			ensureMinSize();

			if (wasResized())
			{
				ImVec2 currentSize = ImGui::GetWindowSize();
				resize((sp_int)currentSize.x, (sp_int)currentSize.y);
			}

			_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
			_windowPosition = ImGui::GetWindowPos();

			return isOpened;
		}

		API_INTERFACE inline void end()
		{
			ImGui::End();

			ImGuiIO io = ImGui::GetIO();
			const sp_bool mouseHovered = isMouseHovered();
			const ImVec2 mouseScreenPos = screenPosition(io.MousePos);

			if (mouseHovered && !wasMouseHovered)
			{
				previousMousePosition = io.MousePos;

				if (onMouseEnter != nullptr)
					onMouseEnter(this, mouseScreenPos);
			}

			if (!mouseHovered && wasMouseHovered)
			{
				if (onMouseLeave != nullptr)
					onMouseLeave(this, mouseScreenPos);
			}

			if (mouseHovered && io.MouseDown[0] && !wasMouseLeftButtonDown && onMouseLeftButtonDown != nullptr)
			{
				_wasMouseDownStartedThisWindow = true;
				onMouseLeftButtonDown(this, mouseScreenPos);
			}

			if (wasMouseLeftButtonDown && io.MouseReleased[0] && onMouseLeftButtonReleased != nullptr)
			{
				if (mouseHovered)
					onMouseLeftButtonReleased(this, mouseScreenPos);
				else
				{
					const ImVec2 pos = screenPosition(previousMousePosition);
					onMouseLeftButtonReleased(this, pos);
				}

				_wasMouseDownStartedThisWindow = false;
			}

			if (mouseHovered 
				&& (previousMousePosition.x != io.MousePos.x || previousMousePosition.y != io.MousePos.y) 
				&& onMouseMove != nullptr)
			{
				const ImVec2 previousScreenPos = screenPosition(previousMousePosition);
				onMouseMove(this, previousScreenPos, mouseScreenPos);
			}

			if (mouseHovered)
				previousMousePosition = io.MousePos;

			wasMouseHovered = mouseHovered;
			wasMouseLeftButtonDown = io.MouseDown[0];
		}

		API_INTERFACE inline virtual void resize(const sp_int width, const sp_int height) noexcept
		{
			this->_width = _minWidth != ZERO_FLOAT && _width < (sp_int)_minWidth && width < _minWidth 
				? (sp_int)_minWidth
				: width;

			this->_height = _minHeight != ZERO_FLOAT && _height < (sp_int)_minHeight && height < _minHeight
				? (sp_int)_minHeight
				: height;
		}

		/// <summary>
		/// Check the window is visible/hidden
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_bool isVisible() const noexcept
		{
			return _visible;
		}

		/// <summary>
		/// Check the window is focused/active
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_bool isFocused() const
		{
			return _isFocused;
		}

		/// <summary>
		/// Show this window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void show() noexcept
		{
			_visible = true;
		}

		/// <summary>
		/// Hide this window
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void hide() noexcept
		{
			_visible = false;
		}

	};
}

#endif // SP_I_FRAME_COMPONENT_HEADER