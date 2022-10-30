#pragma once
#include <map>
#include <memory>

#include "ISkeleton.h"
#include "graphics/skinned_mesh_instance.h"
#include "system/string_id.h"

using gef::StringId;
using std::unique_ptr;

namespace AnimationSystem
{
    class SkinnedMeshContainer
    {
    public:
        void CreateSkinnedMesh(ISkeleton const& skeleton);
        

    private:
        std::map<StringId, unique_ptr<gef::SkinnedMeshInstance>> skinned_meshes_;
    };
}