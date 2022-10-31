#pragma once
#include <map>
#include <memory>

#include "MeshWrapper.h"
#include "SkeletonWrapper.h"
#include "SkinnedMeshWrapper.h"
#include "system/string_id.h"

using gef::StringId;
using std::unique_ptr;

namespace AnimationSystem
{
    class SkinnedMeshContainer
    {
    public:
        AnimationSystem::SkinnedMeshWrapper const& CreateSkinnedMesh(SkeletonWrapper const& skeleton,
                                                                     MeshWrapper const& mesh);


        AnimationSystem::SkinnedMeshWrapper* GetSkinnedMesh(StringId id) const;
    private:
        AnimationSystem::SkinnedMeshWrapper const& MakeAndStoreMesh(SkeletonWrapper const& skeleton);
        std::map<StringId, unique_ptr<SkinnedMeshWrapper>> skinned_meshes_;
    };
}