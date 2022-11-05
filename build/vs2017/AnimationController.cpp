#include "AnimationController.h"

AnimationSystem::AnimationController::AnimationController(StringId const id, gef::Platform const& platform)
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
    const std::string& filePath, const std::string& nameWithinFile, const std::function<void(AnimatorConfig)> configDelegate)
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

AnimationSystem::PureResult AnimationSystem::AnimationController::UpdateAnimation(
    float frameTime, gef::SkinnedMeshInstance& skinnedMesh)
{
    if(!current_animation_.has_value())
        return PureResult::Error(ERROR_TAG+"Cannot animate as no animation is selected");

    animator_->Update(frameTime, skinnedMesh.bind_pose());
    skinnedMesh.UpdateBoneMatrices(animator_->pose());
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(Animation& animation)
{
    current_animation_ = animation;
    
    auto & animator = *animator_;
    animator.set_clip(&animation.GetAnimation());
    
    try
    {
        auto animatorConfigWrapper = AnimatorConfig{animator};
        animation.ApplyConfig(animatorConfigWrapper);
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

