﻿#include "AnimationController.h"

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

AnimationSystem::PureResult AnimationSystem::AnimationController::CreateAnimation(string const & animationName,
    std::string const & filePath, std::string const & nameWithinFile, std::optional<std::function<void(IAnimatorConfig&)> const> const configDelegate)
{
    return animations_->LoadAnimations(animationName, filePath, nameWithinFile, configDelegate);
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(const std::string animationName, float transitionTime)
{
    const auto id = gef::GetStringId(animationName);
    if(!animations_->HasAnimation(id))
        return PureResult::Error(ERROR_TAG+"Animation '" + animationName + "' could not be found");
    return SetAnimation(id);
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(StringId const animationId, float transitionTime)
{
    const auto findAnimationResult = animations_->GetAnimation(animationId);
    if(findAnimationResult.IsError())
        return findAnimationResult.ToPureResult();

    auto const & animation = findAnimationResult.Get().get();

    auto result = SetAnimation(animation, transitionTime);
    
    return result;
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(Animation const & animation, float const transitionTime)
{
    current_animation_name_ = animation.ID();

    return animator_->SetAnimation(animation, transitionTime);
}

AnimationSystem::ValueResult<gef::SkeletonPose> AnimationSystem::AnimationController::UpdateAnimation(
    const float frameTime, gef::SkeletonPose const &skeletonPose)
{
    if(!current_animation_name_.has_value())
        return ValueResult<gef::SkeletonPose>::Error(ERROR_TAG+"Cannot animate as no animation is selected");

    auto newPose = animator_->UpdateAnimation(frameTime, skeletonPose);
    return ValueResult<gef::SkeletonPose>::OK(std::move(newPose));
}
