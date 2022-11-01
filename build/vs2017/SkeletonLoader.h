#pragma once
#include "SkeletonWrapper.h"
#include "Result.h"
#include "graphics/scene.h"


namespace AnimationSystem
{
    class ISkeletonLoader
    {
    public:
        virtual ~ISkeletonLoader() = default;
        virtual Result LoadSkeletonScene(gef::Scene& scene) = 0;
        virtual SkeletonWrapper const * GetSkeleton(std::string const& name) const = 0;
        virtual SkeletonWrapper const * GetSkeleton(unsigned id) const = 0;
        virtual std::vector<unsigned int> GetAllSkeletonIDs() const = 0;
    };
    
}
namespace AnimationSystem
{
    class SkeletonLoader final : public ISkeletonLoader
    {
    public:
        explicit SkeletonLoader(gef::Platform& platform_);
        Result LoadSkeletonScene(gef::Scene& scene) override;
        SkeletonWrapper const * GetSkeleton(std::string const& name) const override;
        SkeletonWrapper const * GetSkeleton(unsigned id) const override;
        std::vector<unsigned> GetAllSkeletonIDs() const override;

    private:
        gef::Platform& platform_;
		
        std::map<gef::StringId, std::unique_ptr<SkeletonWrapper>> skeletons_;
    };

    class SingleSkeletonLoader
    {
    public:
        Result LoadSkeleton(StringId const id, gef::Scene const& scene, gef::Platform& platform);
        std::unique_ptr<SkeletonWrapper> TakeSkeleton() {return std::move(skeleton_); }

    private:
        std::unique_ptr<SkeletonWrapper> skeleton_;
    };
}