#pragma once
#include <sys/stat.h>

namespace AnimationSystem{
    namespace AnimationMath
    {
        static constexpr float InverseLerp(float const begin, float const end, float const value)
        {
            return (value - begin)/(end - begin);
        }
    }
}
