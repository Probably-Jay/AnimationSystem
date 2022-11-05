#include "AnimatedObject.h"

#include "graphics/renderer_3d.h"

// AnimationSystem::Result AnimationSystem::AnimatedObject::Create(AnimatedObject& item,
//     gef::Platform& platform, unique_ptr<gef::Scene> modelScene)
// {
//     return item.Create(platform, std::move(modelScene));
// }

AnimationSystem::PureResult AnimationSystem::AnimatedObject::CreateObjectsFromScene(gef::Platform& platform,
                                                                std::unique_ptr<gef::Scene> modelScene)
{
    modelScene->CreateMaterials(platform);

    if(auto result = skinnedMeshContainer.CreateSkinnedMesh(name_id_,platform, std::move(modelScene)); result.IsError())
        return result;

    const auto skinnedMesh = skinnedMeshContainer.SkinnedMesh();
    animator->Init(skinnedMesh);
    
    return PureResult::OK();
}

void AnimationSystem::AnimatedObject::UpdateAnimation(float frameTime)
{
    animator->UpdateAnimation(frameTime, skinnedMeshContainer.SkinnedMesh());
}

void AnimationSystem::AnimatedObject::set_transform(const gef::Matrix44& transform)
{
    skinnedMeshContainer.SkinnedMesh().set_transform(transform);
}

void AnimationSystem::AnimatedObject::RenderSelf(gef::Renderer3D& renderer3D)
{
    renderer3D.DrawSkinnedMesh(skinnedMeshContainer.SkinnedMesh(), skinnedMeshContainer.SkinnedMesh().bone_matrices());
}
