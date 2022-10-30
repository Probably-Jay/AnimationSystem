#pragma once
#include "IProtectedWrapper.h"
#include "graphics/skinned_mesh_instance.h"

namespace AnimationSystem
{
    typedef IProtectedWrapper<gef::SkinnedMeshInstance> ISkinnedMesh;
    class GefSkinnedMeshWrapper : ISkinnedMesh
    {
    public:
        static std::unique_ptr<ISkinnedMesh> Create(gef::Skeleton const & skinnedMesh, const gef::StringId id)
        {
            auto iSkeleton = std::unique_ptr<ISkinnedMesh>{ new GefSkinnedMeshWrapper{skinnedMesh, id}};
            return iSkeleton;
        }

        gef::SkinnedMeshInstance const& Item() const override {return skinned_mesh_;}
        gef::StringId ID() const override {return id_;}

    private:
        GefSkinnedMeshWrapper(gef::SkinnedMeshInstance const & skinnedMesh, const gef::StringId id)
                    : skinned_mesh_(skinnedMesh), id_(id)
        {
        }
        
        gef::SkinnedMeshInstance skinned_mesh_;
        UInt32 id_;
    };
}
