#include "SkeletonLoader.h"


AnimationSystem::PureResult AnimationSystem::SkeletonLoader::LoadSkeleton(gef::StringId const id, gef::Scene const& scene, gef::Platform & platform)
{
    const auto & skeletons = scene.skeletons;
    
    if(skeletons.empty())
        return PureResult::Error("Scene contained no skeletons");

    if(skeletons.size() > 1)
        return PureResult::Error("Multiple skeletons per scene is not supported");

    const auto skeletonFromFile = skeletons.front();
    skeleton_ = *skeletonFromFile;

    return PureResult::OK();
}
