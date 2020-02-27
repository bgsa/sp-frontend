#ifndef ABOUT_FRAME_HEADER
#define ABOUT_FRAME_HEADER

#include "apollo.h"
#include "imgui/imgui.h"

class AboutFrame
{
    private:
        sp_bool showWindow = false;

    public:

        API_INTERFACE sp_bool isVisible();

        API_INTERFACE void show();
        API_INTERFACE void hide();

        API_INTERFACE void render();

};

#endif // ABOUT_FRAME_HEADER