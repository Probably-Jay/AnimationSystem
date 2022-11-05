#include "AnimationController.h"

AnimationSystem::AnimationController::AnimationController(StringId const id, gef::Platform const& platform)
    : currentAnimationId(nullptr)
    , animations_(new AnimationContainer{platform})
    , animator_(AnimatorWrapper::Create(id))
{
}

void AnimationSystem::AnimationController::Init(SkinnedMeshWrapper const* skinnedMesh)
{
    animator_->Item().Init(skinnedMesh->Item().bind_pose());
}

AnimationSystem::PureResult AnimationSystem::AnimationController::CreateAnimation(const string& animationName,
    const std::string& filePath, const std::string& nameWithinFile, const std::function<void(AnimatorConfig&)>& configDelegate)
{
    animations_->LoadAnimations(animationName, filePath, nameWithinFile, configDelegate);
    return PureResult::Error(ERROR_TAG+"Not implimented");
}

AnimationSystem::PureResult AnimationSystem::AnimationController::SetAnimation(const std::string animationName)
{
    const auto id = gef::GetStringId(animationName);
    if(!animations_->HasAnimation(id))
        return PureResult::Error(ERROR_TAG+"Animation" + animationName + "could not be found");
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
    auto & animator = animator_->Item();

    try
    {
        current_animation_.value().get().ApplyConfig(AnimatorConfig{animator});
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


// AnimationSystem::Result AnimationSystem::AnimationController::SetCurrentAnimation(const StringId id)
// {
//     if(!animations_.HasAnimation(id))
//         return Result::Error(ERROR_TAG+ "Animation collection does not contain id" + std::to_string(id) );
//
//     currentAnimationId = id;
//     return Result::OK();
// }
