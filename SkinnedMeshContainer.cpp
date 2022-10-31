#include "SkinnedMeshContainer.h"

#include "MeshWrapper.h"

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
