#include "AnimatedObject.h"

// AnimationSystem::Result AnimationSystem::AnimatedObject::Create(AnimatedObject& item,
//     gef::Platform& platform, unique_ptr<gef::Scene> modelScene)
// {
//     return item.Create(platform, std::move(modelScene));
// }

AnimationSystem::Result AnimationSystem::AnimatedObject::CreateObjectsFromScene(gef::Platform& platform,
                                                                std::unique_ptr<gef::Scene> modelScene)
{
    modelScene->CreateMaterials(platform);
    auto result = skinnedMeshContainer.CreateSkinnedMesh(name_id_,platform, std::move(modelScene));

    if(result.IsError())
        return result;

    const auto skinnedMesh = skinnedMeshContainer.SkinnedMesh();
    animator->Item().Init(skinnedMesh);
    
    return Result::OK();
}
