#ifndef PROPERTIES_FRAME_HEADER
#define PROPERTIES_FRAME_HEADER

#include "AbstractFrame.h"

class PropertiesFrame
    : public AbstractFrame
{
    public:

        API_INTERFACE void init() override;

        API_INTERFACE void render() override;
};

#endif // PROPERTIES_FRAME_HEADER