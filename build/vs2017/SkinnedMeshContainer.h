#pragma once
#include <map>
#include <memory>

#include "MeshWrapper.h"
#include "Result.h"
#include "SkeletonWrapper.h"
#include "SkinnedMeshWrapper.h"
#include "graphics/scene.h"
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

    class SingleSkinnedMeshContainer
    {
    public:
        AnimationSystem::Result Create(const StringId id, gef::Platform& platform);
        Result CreateSkinnedMesh(StringId id, gef::Platform& platform, std::unique_ptr<gef::Scene> modelScene);
        SkinnedMeshWrapper * SkinnedMesh() const {return skinned_mesh_.get();}
    private:
        unique_ptr<MeshWrapper> mesh_ = nullptr;
        unique_ptr<SkeletonWrapper> skeleton_ = nullptr;
        
        unique_ptr<SkinnedMeshWrapper> skinned_mesh_ = nullptr;
        unique_ptr<gef::Scene> model_scene_;
    };

   
}