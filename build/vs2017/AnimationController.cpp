#include "AnimationController.h"

AnimationSystem::AnimationController::AnimationController(IAnimationContainer const& animations)
    : animations_(animations), currentAnimationId(0)
{
}

AnimationSystem::Result AnimationSystem::AnimationController::SetCurrentAnimation(const StringId id)
{
    if(!animations_.HasAnimation(id))
        return Result::Error(ERROR_TAG+ "Animation collection does not contain id" + std::to_string(id) );

    currentAnimationId = id;
    return Result::OK();
}
