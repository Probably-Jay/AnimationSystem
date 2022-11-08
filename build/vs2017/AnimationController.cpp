#include "AnimationController.h"

AnimationSystem::AnimationController::AnimationController(gef::Platform const& platform)
    : current_animation_name_({})
    , animations_(new AnimationContainer{platform})
    , animator_(std::make_unique<BlendAnimator>())
{
}

void AnimationSystem::AnimationController::Init(gef::SkinnedMeshInstance const & skinnedMesh)
{
    animator_->Init(skinnedMesh.bind_pose());
}

AnimationSystem::PureResult AnimationSystem::AnimationController::CreateAnimation(const string& animationName,
    const std::string& filePath, const std::string& nameWithinFile, std::optional<std::function<void(IAnimatorConfig&)> const> const configDelegate)
{
    return animations_->LoadAnimations(animationName, filePath, nameWithinFile, configDelegate);
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(const std::string animationName)
{
    const auto id = gef::GetStringId(animationName);
    if(!animations_->HasAnimation(id))
        return PureResult::Error(ERROR_TAG+"Animation '" + animationName + "' could not be found");
    return SetAnimation(id);
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(const StringId animationId)
{
    const auto result = animations_->GetAnimation(animationId);
    if(result.IsError())
        return result.ToPureResult();

    auto & animation = result.Get().get();
    SetAnimation(animation);
    
    return result.ToPureResult();
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(Animation& animation)
{
    current_animation_name_ = animation.ID();

    return animator_->ActiveAnimator().SetAnimation(animation);
}

AnimationSystem::ValueResult<gef::SkeletonPose> AnimationSystem::AnimationController::UpdateAnimation(
    const float frameTime, gef::SkeletonPose const &skeletonPose)
{
    if(!current_animation_name_.has_value())
        return ValueResult<gef::SkeletonPose>::Error(ERROR_TAG+"Cannot animate as no animation is selected");

    return ValueResult<gef::SkeletonPose>::OK(animator_->ActiveAnimator().UpdateAnimation(frameTime, skeletonPose));
}
