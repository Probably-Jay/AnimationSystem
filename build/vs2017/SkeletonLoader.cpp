﻿#include "SkeletonLoader.h"

AnimationSystem::SkeletonLoader::SkeletonLoader(gef::Platform& platform_)
    : platform_(platform_)
{
}

Result AnimationSystem::SkeletonLoader::LoadSkeletonScene(gef::Scene& scene)
{
    const auto & meshDataList = scene.mesh_data;
    const auto & skeletons = scene.skeletons;
    
    if(skeletons.empty())
        return Result::Error("Scene contained no skeletons");


    
    //todo gef's way of getting the actual ID is broken, make it up
    for (const auto skeleton : skeletons)
    {
        auto wrappedSkeleton = GefSkeletonWrapper::Create(*skeleton);

        gef::StringId uniqueID = reinterpret_cast<unsigned>(&wrappedSkeleton->Skeleton());  // NOLINT
        
        skeletons_.emplace(uniqueID, std::move(wrappedSkeleton));
    }

    return Result::OK();
    
    for (auto & meshData : meshDataList)
    {
        const auto skeleton = scene.FindSkeleton(meshData);
        
        if(skeleton == nullptr)
            return Result::Error("Skeleton " + std::to_string(meshData.name_id) + "could not be found");
    
        auto wrappedSkeleton = GefSkeletonWrapper::Create(*skeleton);
        skeletons_.emplace(meshData.name_id, std::move(wrappedSkeleton));
    }
	
    return Result::OK();
}

AnimationSystem::ISkeleton const* AnimationSystem::SkeletonLoader::GetSkeleton(std::string const& name) const
{
    return GetSkeleton(gef::GetStringId(name));
}

AnimationSystem::ISkeleton const* AnimationSystem::SkeletonLoader::GetSkeleton(gef::StringId id) const
{
    const auto skeleton = skeletons_.find(id );

    if(skeleton == skeletons_.end())
        return nullptr;
	
    return skeleton->second.get();
}

std::vector<unsigned> AnimationSystem::SkeletonLoader::GetAllSkeletonIDs() const
{
    std::vector<unsigned int> IDs;
    for(auto const& pair: skeletons_)
        IDs.push_back(pair.first);
    return IDs;
}
