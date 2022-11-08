#include "AnimatedObject.h"

#include "graphics/renderer_3d.h"

AnimationSystem::AnimatedObject::AnimatedObject(gef::Platform& platform, const StringId nameId)
    : name_id_(nameId)
    , animator_(std::make_unique<AnimationController>(platform))
{}

AnimationSystem::PureResult AnimationSystem::AnimatedObject::CreateObjectsFromScene(gef::Platform& platform,
                                                                                    std::unique_ptr<gef::Scene> modelScene)
{
    modelScene->CreateMaterials(platform);

    if(auto result = skinned_mesh_container_.CreateSkinnedMesh(name_id_,platform, std::move(modelScene)); result.IsError())
        return result;

    const auto skinnedMesh = skinned_mesh_container_.SkinnedMesh();
    animator_->Init(skinnedMesh);
    
    return PureResult::OK();
}

AnimationSystem::PureResult AnimationSystem::AnimatedObject::UpdateAnimation(float frameTime)
{
    auto newPoseResult = animator_->UpdateAnimation(frameTime, skinned_mesh_container_.SkinnedMesh().bind_pose());
    if(newPoseResult.IsError())
        return newPoseResult.ToPureResult();
    
    const auto newPose = newPoseResult.Take();
    skinned_mesh_container_.SkinnedMesh().UpdateBoneMatrices(newPose);
    return PureResult::OK();
}

void AnimationSystem::AnimatedObject::SetTransform(const gef::Matrix44& transform)
{
    skinned_mesh_container_.SkinnedMesh().set_transform(transform);
}

void AnimationSystem::AnimatedObject::RenderSelf(gef::Renderer3D& renderer3D)
{
    renderer3D.DrawSkinnedMesh(skinned_mesh_container_.SkinnedMesh(), skinned_mesh_container_.SkinnedMesh().bone_matrices());
}
