#pragma once
#include "animation/skeleton.h"
#include "IProtectedWrapper.h"

namespace AnimationSystem
{
    typedef IProtectedWrapper<gef::Skeleton> ISkeleton;
    class GefSkeletonWrapper final : ISkeleton
    {
    public:
        static std::unique_ptr<ISkeleton> Create(gef::Skeleton const & skeleton, const gef::StringId id)
        {
            auto iSkeleton = std::unique_ptr<ISkeleton>{ new GefSkeletonWrapper{skeleton, id}};
            return iSkeleton;
        }

        gef::Skeleton const & Item() const override {return skeleton_;}
        gef::StringId ID() const override {return id_;}

    private:
        // prevent creation outside of class
        explicit GefSkeletonWrapper(gef::Skeleton const & mesh, const gef::StringId id)
            : skeleton_(mesh), id_(id)
        {
        }

        gef::Skeleton const & skeleton_;
        const gef::StringId id_;

        void operator delete(void * p){::operator delete(p);}
    };
}