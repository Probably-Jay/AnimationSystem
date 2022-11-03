#include "AnimationController.h"

AnimationSystem::AnimationController::AnimationController(StringId const id, gef::Platform const& platform)
    : currentAnimationId(nullptr)
    , animations_(new AnimationContainer{platform})
    , animator(AnimatorWrapper::Create(id))
{
}

void AnimationSystem::AnimationController::Init(SkinnedMeshWrapper const* skinnedMesh)
{
    animator->Item().Init(skinnedMesh->Item().bind_pose());
}

// AnimationSystem::Result AnimationSystem::AnimationController::SetCurrentAnimation(const StringId id)
// {
//     if(!animations_.HasAnimation(id))
//         return Result::Error(ERROR_TAG+ "Animation collection does not contain id" + std::to_string(id) );
//
//     currentAnimationId = id;
//     return Result::OK();
// }
