#pragma once
#include "ISkeleton.h"
#include "Result.h"
#include "graphics/scene.h"


namespace AnimationSystem
{
    class ISkeletonLoader
    {
    public:
        virtual ~ISkeletonLoader() = default;
        virtual Result LoadSkeletonScene(gef::Scene& scene) = 0;
        virtual ISkeleton const * GetSkeleton(std::string const& name) const = 0;
        virtual ISkeleton const * GetSkeleton(unsigned id) const = 0;
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
        ISkeleton const * GetSkeleton(std::string const& name) const override;
        ISkeleton const * GetSkeleton(unsigned id) const override;
        std::vector<unsigned> GetAllSkeletonIDs() const override;

    private:
        gef::Platform& platform_;
		
        std::map<gef::StringId, std::unique_ptr<ISkeleton>> skeletons_;
    };
}