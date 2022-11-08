#include "AnimationController.h"

AnimationSystem::AnimationController::AnimationController(gef::Platform const& platform)
    : current_animation_({})
    , animations_(new AnimationContainer{platform})
    , animator_(new MotionClipPlayer{})
{
}

void AnimationSystem::AnimationController::Init(gef::SkinnedMeshInstance const & skinnedMesh)
{
    animator_->Init(skinnedMesh.bind_pose());
}

AnimationSystem::PureResult AnimationSystem::AnimationController::CreateAnimation(const string& animationName,
    const std::string& filePath, const std::string& nameWithinFile, std::optional<std::function<void(AnimatorConfig)> const> const configDelegate)
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
    current_animation_ = animation;

    return animator_->SetAnimation(animation);
}

AnimationSystem::PureResult AnimationSystem::AnimationController::UpdateAnimation(
    const float frameTime, gef::SkinnedMeshInstance& skinnedMesh)
{
    if(!current_animation_.has_value())
        return PureResult::Error(ERROR_TAG+"Cannot animate as no animation is selected");

    animator_->UpdateAnimation(frameTime, skinnedMesh);
    return PureResult::OK();
}

void AnimationSystem::AnimatorWrapper::UpdateAnimation(const float frameTime, gef::SkinnedMeshInstance& skinnedMesh)
{
    animator_.Update(frameTime, skinnedMesh.bind_pose());
    skinnedMesh.UpdateBoneMatrices(animator_.pose());
}

AnimationSystem::PureResult AnimationSystem::AnimatorWrapper::SetAnimation(Animation const & animation)
{
    animator_.SetClip(&animation.GetAnimation());
    
    try
    {
        animation.ApplyConfig(animator_);
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

