#include "AnimatorWrapper.h"

gef::SkeletonPose AnimationSystem::AnimatorWrapper::UpdateAnimation(const float frameTime, gef::SkeletonPose const& bindPose)
{
    animator_.Update(frameTime, bindPose);
   
   return animator_.pose();
}

AnimationSystem::PureResult AnimationSystem::AnimatorWrapper::SetAnimation(Animation const & animation)
{
    current_animation_ = animation;
    animator_.set_clip(&animation.GetAnimation());
    return ApplyConfigs(animation);
}

AnimationSystem::PureResult AnimationSystem::AnimatorWrapper::SetAnimation(AnimatorWrapper const& animator)
{
    if(auto const animation = animator.current_animation_; animation.has_value())
        return SetAnimation(animation.value());
    return PureResult::Error(ERROR_TAG+"Animator has no animation");
}


AnimationSystem::PureResult AnimationSystem::AnimatorWrapper::ApplyConfigs(Animation const & animation)
{
    try
    {
        animation.ApplyConfig(*this);
    }
    catch (std::exception& e)
    {
        return PureResult::Error(ERROR_TAG, e);
    }
    catch (...)
    {
        return PureResult::Error(ERROR_TAG+"Unknown exception thrown in user-given animator configuration.");
    }
    return PureResult::OK();
}

