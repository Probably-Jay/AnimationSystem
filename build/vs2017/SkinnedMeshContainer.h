﻿#pragma once
#include <map>
#include <memory>

#include "MeshWrapper.h"
#include "PureResult.h"
#include "SkeletonWrapper.h"
#include "SkinnedMeshWrapper.h"
#include "graphics/scene.h"
#include "system/string_id.h"

using gef::StringId;
using std::unique_ptr;

namespace AnimationSystem
{
    // class SkinnedMeshContainer
    // {
    // public:
    //     SkinnedMeshWrapper const& CreateSkinnedMesh(SkeletonWrapper const& skeleton,
    //                                                 MeshWrapper const& mesh);
    //
    //
    //     SkinnedMeshWrapper* GetSkinnedMesh(StringId id) const;
    // private:
    //     SkinnedMeshWrapper& MakeAndStoreMesh(SkeletonWrapper const& skeleton);
    //     std::map<StringId, unique_ptr<SkinnedMeshWrapper>> skinned_meshes_;
    // };

    class SingleSkinnedMeshContainer
    {
    public:
        PureResult Create(const StringId id, gef::Platform& platform);
        PureResult CreateSkinnedMesh(StringId id, gef::Platform& platform, std::unique_ptr<gef::Scene> modelScene);
        gef::SkinnedMeshInstance & SkinnedMesh() const {return skinned_mesh_->Item();}
    private:
        unique_ptr<MeshWrapper> mesh_ = nullptr;
        unique_ptr<SkeletonWrapper> skeleton_ = nullptr;
        
        unique_ptr<SkinnedMeshWrapper> skinned_mesh_ = nullptr;
        unique_ptr<gef::Scene> model_scene_;
    };

   
}