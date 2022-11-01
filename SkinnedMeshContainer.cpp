#include "SkinnedMeshContainer.h"

#include <pplwin.h>

#include "MeshLoader.h"
#include "MeshWrapper.h"
#include "SkeletonLoader.h"

AnimationSystem::SkinnedMeshWrapper const& AnimationSystem::SkinnedMeshContainer::CreateSkinnedMesh(SkeletonWrapper const& skeleton, MeshWrapper const& mesh)
{
    const auto & sMesh = MakeAndStoreMesh(skeleton);
    sMesh.Item().set_mesh(&mesh.Item());
    return sMesh;
}

AnimationSystem::SkinnedMeshWrapper* AnimationSystem::SkinnedMeshContainer::GetSkinnedMesh(const StringId id) const
{
    const auto sMeshIter = skinned_meshes_.find(id);
    if(sMeshIter == skinned_meshes_.end())
        return nullptr;

    return sMeshIter->second.get();
}

AnimationSystem::SkinnedMeshWrapper const& AnimationSystem::SkinnedMeshContainer::MakeAndStoreMesh(SkeletonWrapper const& skeleton)
{
    auto skinnedMesh = std::make_unique<gef::SkinnedMeshInstance>(gef::SkinnedMeshInstance{skeleton.Item()});
    auto iSkinnedMesh = SkinnedMeshWrapper::Create(std::move(skinnedMesh), skeleton.ID());
    skinned_meshes_.emplace(skeleton.ID(), std::move(iSkinnedMesh));
    return *skinned_meshes_.at(skeleton.ID());
}

AnimationSystem::Result AnimationSystem::SingleSkinnedMeshContainer::CreateSkinnedMesh(StringId id, gef::Platform& platform, std::unique_ptr<gef::Scene> modelScene)
{
    model_scene_ = std::move(modelScene);
    return Create(id, platform);
}

AnimationSystem::Result AnimationSystem::SingleSkinnedMeshContainer::Create(const StringId id, gef::Platform& platform)
{
    auto meshLoader = SingleMeshLoader{};
    {
        auto result = meshLoader.LoadMesh(id, *model_scene_, platform);
    
        if(result.IsError())
            return result;
    }
    
    auto skeletonLoader = SingleSkeletonLoader{};
    {
        auto result = skeletonLoader.LoadSkeleton(id, *model_scene_, platform);
    
        if(result.IsError())
            return result;
    }

    mesh_ = meshLoader.TakeMesh();
    skeleton_ = skeletonLoader.TakeSkeleton();

    skinned_mesh_ = SkinnedMeshWrapper::Create(id, skeleton_->Item());
    skinned_mesh_->Item().set_mesh(&mesh_->Item());

    return Result::OK();
}
