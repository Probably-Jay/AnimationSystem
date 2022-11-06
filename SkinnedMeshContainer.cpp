#include "SkinnedMeshContainer.h"

#include <pplwin.h>

#include "MeshLoader.h"
#include "MeshWrapper.h"
#include "SkeletonLoader.h"

AnimationSystem::PureResult AnimationSystem::SkinnedMeshInstance::CreateSkinnedMesh(StringId id, gef::Platform& platform, std::unique_ptr<gef::Scene> modelScene)
{
    model_scene_ = std::move(modelScene);
    return Create(id, platform);
}

AnimationSystem::PureResult AnimationSystem::SkinnedMeshInstance::Create(const StringId id, gef::Platform& platform)
{
    auto meshLoader = MeshLoader{};
    if(auto result = meshLoader.LoadMesh(id, *model_scene_, platform); result.IsError())
        return result;
    
    
    auto skeletonLoader = SkeletonLoader{};
    if(auto result = skeletonLoader.LoadSkeleton(id, *model_scene_, platform); result.IsError())
        return result;

    const auto & mesh = meshLoader.Mesh();
    auto & skeleton = skeletonLoader.Skeleton();

    skinned_mesh_ = std::make_unique<gef::SkinnedMeshInstance>(skeleton);
    skinned_mesh_->set_mesh(&mesh);

    return PureResult::OK();
}
