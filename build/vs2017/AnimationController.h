#pragma once
#include "IAnimationContainer.h"

namespace AnimationSystem
{
    class AnimationController
    {
        explicit AnimationController(IAnimationContainer const & animations);

        Result SetCurrentAnimation(StringId id);

        void Animate(){throw;}

    private:
        IAnimationContainer const & animations_;
        StringId currentAnimationId;

        AnimationWrapper * CurrentAnimation() const {return animations_.GetAnimation(currentAnimationId);}
    };


}
