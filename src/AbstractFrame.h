#ifndef ABSTRACT_FRAME_HEADER
#define ABSTRACT_FRAME_HEADER

#include "apollo.h"
#include "imgui/imgui.h"

class AbstractFrame
{
    private:
        sp_bool showWindow = false;

    public:

        API_INTERFACE inline sp_bool isVisible()
        {
            return showWindow;
        }

        API_INTERFACE inline void show()
        {
            this->showWindow = true;
        }
        API_INTERFACE inline void hide()
        {
            this->showWindow = false;
        }

        API_INTERFACE virtual void render() = 0;

};

#endif // ABSTRACT_FRAME_HEADER