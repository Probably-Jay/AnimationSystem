#include <BlendAnimator.h>

AnimationSystem::BlendAnimator::BlendAnimator(): current_animator_(Controller::Left) {}

AnimationSystem::AnimatorWrapper& AnimationSystem::BlendAnimator::ActiveAnimator() const
{
    return controllers_.Get(current_animator_);
}

gef::SkeletonPose AnimationSystem::BlendAnimator::UpdateAnimationSimple(float const frameTime,
                                                                        gef::SkeletonPose const &skeletonPose)
{
    return ActiveAnimator().UpdateAnimation(frameTime, skeletonPose);
}

AnimationSystem::ValueResult<gef::SkeletonPose> AnimationSystem::BlendAnimator::UpdateAnimationBlended(
    float const frameTime,
    gef::SkeletonPose const &skeletonPose)
{
    if(auto const result = UpdateTransitionAndApplyTransitionDelegate(frameTime); result.IsError())
        return ValueResult<gef::SkeletonPose>::Error(result.Error());

    if (!InTransition()) // transition just ended this update
    {
        auto pose = UpdateAnimationSimple(frameTime, skeletonPose);
        return  ValueResult<gef::SkeletonPose>::OK(std::move(pose));
    }

    auto const activePose = ActiveAnimator().UpdateAnimation(frameTime, skeletonPose);
    auto const previousPose = AlternateAnimator().UpdateAnimation(frameTime, skeletonPose);

    auto const lerpValue = transition_->LerpValue();
        // todo
    auto blendedPose = skeletonPose;
    blendedPose.Linear2PoseBlend(previousPose, activePose, lerpValue);

    return ValueResult<gef::SkeletonPose>::OK(std::move(blendedPose));
}

AnimationSystem::PureResult AnimationSystem::BlendAnimator::UpdateTransitionAndApplyTransitionDelegate(
    float const frameTime)
{
    transition_->AccumulateTime(frameTime);

    if(auto result =ApplyTransitionDelegate(); result.IsError())
        return result;

    // transition is over, reset transition object
    if (!transition_->InTransition())
        transition_.reset();
    
    return PureResult::OK();
}

AnimationSystem::PureResult AnimationSystem::BlendAnimator::ApplyTransitionDelegate()
{
    try
    {
        transition_->ApplyDelegate(ActiveAnimator());
    }
    catch (std::exception& e)
    {
        return PureResult::Error(ERROR_TAG, e);
    }
    catch (...)
    {
        return PureResult::Error(ERROR_TAG+"Unknown exception thrown in user-given animator blend configuration.");
    }
    return PureResult::OK();
}

void AnimationSystem::BlendAnimator::SyncAnimators()
{
    auto const percentThroughAnimation = ActiveAnimator().GetPercentThroughAnimation();
    AlternateAnimator().SetAnimationTimePercent(percentThroughAnimation.value_or(0));
}

AnimationSystem::PureResult AnimationSystem::BlendAnimator::SetAnimationSimple(Animation const &animation)
{
    return ActiveAnimator().SetAnimation(animation);
}

gef::SkeletonPose AnimationSystem::BlendAnimator::UpdateAnimation(float const frameTime,
                                                                  gef::SkeletonPose const &skeletonPose)
{
    if (!InTransition())
        return UpdateAnimationSimple(frameTime, skeletonPose);

    auto blendedPoseResult = UpdateAnimationBlended(frameTime, skeletonPose);
    if(blendedPoseResult.IsError())
        blendedPoseResult.Raise();
    
    return blendedPoseResult.Take();
}

AnimationSystem::PureResult AnimationSystem::BlendAnimator::SetAnimation(Animation const& animation,
    float transitionTime, Animation::OptionalConfigOnTransitionAnimationDelegate transitionDelegate) {
    if(transitionTime <= 0)
        return SetAnimationSimple(animation);

    SyncAnimators();
            
    if(auto setAltResult = AlternateAnimator().SetAnimation(ActiveAnimator()); setAltResult.IsError())
        return setAltResult;
            
    if(auto setActiveResult = ActiveAnimator().SetAnimation(animation); setActiveResult.IsError())
        return setActiveResult;
            
    transition_ = std::make_unique<Transition>(transitionTime, std::move(transitionDelegate));
    return PureResult::OK();
}

AnimationSystem::AnimatorWrapper& AnimationSystem::BlendAnimator::AlternateAnimator() const
{
    return controllers_.Get(ControllerOther(current_animator_));
}

bool AnimationSystem::BlendAnimator::InTransition() const { return transition_ != nullptr; }

AnimationSystem::AnimatorWrapper& AnimationSystem::BlendAnimator::AnimationControllerPair::Get(
    Controller const controller) const { return map_.at(controller)(); }

AnimationSystem::BlendAnimator::Controller AnimationSystem::BlendAnimator::ControllerOther(Controller const current)
{
    switch (current)
    {
    case Controller::Left: return Controller::Right;
    case Controller::Right: return Controller::Left;
    }
    throw;
}

void AnimationSystem::BlendAnimator::Init(gef::SkeletonPose const &bindPose)
{
    ActiveAnimator().Init(bindPose);
    AlternateAnimator().Init(bindPose);
}
