#include "SkinnedMeshContainer.h"

void AnimationSystem::SkinnedMeshContainer::CreateSkinnedMesh(ISkeleton const& skeleton)
{
    auto skinnedMesh = std::make_unique<gef::SkinnedMeshInstance>(gef::SkinnedMeshInstance{skeleton.Item()});
    skinned_meshes_.emplace(skeleton.ID(), std::move(skinnedMesh));
}
