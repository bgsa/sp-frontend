#ifndef LOG_FRAME_HEADER
#define LOG_FRAME_HEADER

#include "AbstractFrame.h"
#include <vector>

class LogFrame
    : public AbstractFrame
{
    private:
        std::vector<sp_char*> messages;

    public:

        API_INTERFACE void init() override;

        API_INTERFACE void render() override;

};

#endif