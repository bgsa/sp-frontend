#ifndef ABOUT_FRAME_HEADER
#define ABOUT_FRAME_HEADER

#include "AbstractFrame.h"

class AboutFrame
    : public AbstractFrame
{
    public:
        API_INTERFACE void render() override;

};

#endif // ABOUT_FRAME_HEADER