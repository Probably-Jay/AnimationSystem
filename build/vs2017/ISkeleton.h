#pragma once
#include "animation/skeleton.h"

namespace AnimationSystem
{
    class ISkeleton
    {
    public:
        virtual ~ISkeleton() = default;

        // todo remove gef dependency?
        virtual gef::Skeleton const & Skeleton() const = 0;

    protected:
        ISkeleton() = default; // prevent creation outside of class
	
        private:
        void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

        friend struct std::default_delete<ISkeleton>; // allow deletion by smart pointer
    };
}

namespace AnimationSystem
{

    class GefSkeletonWrapper final : ISkeleton
    {
    public:
        static std::unique_ptr<ISkeleton> Create(gef::Skeleton const & skeleton)
        {
            auto iSkeleton = std::unique_ptr<ISkeleton>{ new GefSkeletonWrapper{skeleton}};
            return iSkeleton;
        }

        gef::Skeleton const & Skeleton() const override {return skeleton_;}


    private:
        // prevent creation outside of class
        explicit GefSkeletonWrapper(gef::Skeleton const & mesh)
            :skeleton_(mesh)
        {
        }

        gef::Skeleton const & skeleton_;

        void operator delete(void * p){::operator delete(p);}
    };
}