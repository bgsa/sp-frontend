#ifndef PROJECT_EXPLORER_FRAME_HEADER
#define PROJECT_EXPLORER_FRAME_HEADER

#include "AbstractFrame.h"
#include <algorithm>

class ProjectExplorerFrame
    : public AbstractFrame
{
    public:

        API_INTERFACE void init() override;

        API_INTERFACE void render() override;
        
};

#endif // PROJECT_EXPLORER_FRAME_HEADER