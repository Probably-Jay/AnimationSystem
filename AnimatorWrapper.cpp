#include "AnimatorWrapper.h"

void AnimationSystem::AnimatorWrapper::UpdateAnimation(const float frameTime, gef::SkinnedMeshInstance& skinnedMesh)
{
    animator_.Update(frameTime, skinnedMesh.bind_pose());
    skinnedMesh.UpdateBoneMatrices(animator_.pose());
}

AnimationSystem::PureResult AnimationSystem::AnimatorWrapper::SetAnimation(Animation const & animation)
{
    animator_.set_clip(&animation.GetAnimation());
    return ApplyConfigs(animation);
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

