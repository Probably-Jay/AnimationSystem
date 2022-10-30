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
        static std::unique_ptr<ISkeleton> Create(std::unique_ptr<gef::Skeleton> skeleton)
        {
            auto iSkeleton = std::unique_ptr<ISkeleton>{ new GefSkeletonWrapper{std::move(skeleton)}};
            return iSkeleton;
        }

        gef::Skeleton const & Skeleton() const override {return *skeleton_;}


    private:
        // prevent creation outside of class
        explicit GefSkeletonWrapper(std::unique_ptr<gef::Skeleton> mesh)
            :skeleton_(std::move(mesh))
        {
        }

        std::unique_ptr<gef::Skeleton> skeleton_;

        void operator delete(void * p){::operator delete(p);}
    };
}