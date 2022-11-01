#include "AnimatedObject.h"

AnimationSystem::Result AnimationSystem::AnimatedObject::Create(gef::Platform& platform,
    std::unique_ptr<gef::Scene> modelScene)
{
    modelScene->CreateMaterials(platform);
    auto result = skinnedMeshContainer.CreateSkinnedMesh(NameId,platform, std::move(modelScene));
    return result;
}
