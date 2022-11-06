#pragma once
#include "SkeletonWrapper.h"
#include "PureResult.h"
#include "graphics/scene.h"


namespace AnimationSystem
{
}
namespace AnimationSystem
{
    class SkeletonLoader
    {
    public:
        PureResult LoadSkeleton(gef::StringId id, gef::Scene const& scene, gef::Platform& platform);
        [[nodiscard]] gef::Skeleton & Skeleton()const {return skeleton_.value().get(); }

    private:
        std::optional<std::reference_wrapper<gef::Skeleton>> skeleton_ = {};
    };
}