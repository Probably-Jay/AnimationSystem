#pragma once
#include <string>

namespace AnimationSystem
{
    typedef unsigned int StringId;
    class IStringID
    {
    public:
        virtual ~IStringID() = default;
        virtual StringId ID() const =0;
    };

    
}
