#ifndef ABSTRACT_FRAME_HEADER
#define ABSTRACT_FRAME_HEADER

#include "apollo.h"
#include "imgui/imgui.h"

class AbstractFrame
{
    private:
        sp_bool showWindow = false;
        sp_int width = 300;
        sp_int height = 200;

    public:

        API_INTERFACE inline ImVec2 getWindowSize() noexcept
        {
            return ImGui::GetIO().DisplaySize;
        }

        API_INTERFACE inline sp_bool isVisible() noexcept
        {
            return showWindow;
        }

        API_INTERFACE inline void show() noexcept
        {
            this->showWindow = true;
        }
        API_INTERFACE inline void hide() noexcept
        {
            this->showWindow = false;
        }

        API_INTERFACE inline sp_int getWidth() noexcept
        {
            return width;
        }
        API_INTERFACE void inline setWidth(sp_int width) noexcept
        {
            this->width = width;
        }

        API_INTERFACE inline sp_int getHeight() noexcept
        {
            return height;
        }
        API_INTERFACE void inline setHeight(sp_int height) noexcept
        {
            this->height = height;
        }

        API_INTERFACE inline void resize(sp_int width, sp_int height) noexcept
        {
            setWidth(width);
            setHeight(height);
        }

        API_INTERFACE virtual void init()
        {
            ;
        };
        
        API_INTERFACE virtual void render() = 0;

};

#endif // ABSTRACT_FRAME_HEADER