#pragma once
#include "AnimatorWrapper.h"
#include "IAnimationContainer.h"
#include "SkinnedMeshWrapper.h"

namespace AnimationSystem
{
    /**
     * \brief unused
     */
    class AnimationController
    {
    public:
        AnimationController(StringId id, gef::Platform const& platform);
      //  Result SetCurrentAnimation(StringId id);

        void Animate(){throw;}
        void Init(SkinnedMeshWrapper const * skinnedMesh );
    private:
        AnimationWrapper * currentAnimationId;
        
        std::unique_ptr<IAnimationContainer> animations_;
        std::unique_ptr<AnimatorWrapper> animator;

      //  AnimationWrapper * CurrentAnimation() const {return animations_.GetAnimation(currentAnimationId);}
    };

    typedef OwningProtectedWrapper<AnimationController> AnimationControllerWrapper;

}
