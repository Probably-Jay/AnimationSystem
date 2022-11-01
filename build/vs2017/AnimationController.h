#pragma once
#include "AnimatorWrapper.h"
#include "IAnimationContainer.h"

namespace AnimationSystem
{
    /**
     * \brief unused
     */
    class AnimationController
    {
        explicit AnimationController(IAnimationContainer const & animations);

        Result SetCurrentAnimation(StringId id);

        void Animate(){throw;}

    private:
        IAnimationContainer const & animations_;
        StringId currentAnimationId;
        std::unique_ptr<AnimatorWrapper> animator;

        AnimationWrapper * CurrentAnimation() const {return animations_.GetAnimation(currentAnimationId);}
    };

    typedef OwningProtectedWrapper<AnimationController> AnimationControllerWrapper;

}
