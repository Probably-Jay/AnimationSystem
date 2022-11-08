#pragma once
#include <memory>

#include "PureResult.h"
#include "graphics/scene.h"
#include "graphics/skinned_mesh_instance.h"
#include "system/string_id.h"

using gef::StringId;
using std::unique_ptr;

namespace AnimationSystem
{
    class SkinnedMeshInstance
    {
    public:
        PureResult CreateSkinnedMesh(StringId id, gef::Platform& platform, std::unique_ptr<gef::Scene> modelScene);
        [[nodiscard]] gef::SkinnedMeshInstance & SkinnedMesh() const {return *skinned_mesh_;}
    private:
        PureResult Create(const StringId id, gef::Platform& platform);
             
        unique_ptr<gef::SkinnedMeshInstance> skinned_mesh_ = nullptr;
        unique_ptr<gef::Scene> model_scene_{};
    };

   
}