#include "SkeletonLoader.h"

AnimationSystem::SkeletonLoader::SkeletonLoader(gef::Platform& platform_)
    : platform_(platform_)
{
}

AnimationSystem::Result AnimationSystem::SkeletonLoader::LoadSkeletonScene(gef::Scene& scene)
{
    const auto & meshDataList = scene.mesh_data;
    const auto & skeletons = scene.skeletons;
    
    if(skeletons.empty())
        return Result::Error("Scene contained no skeletons");
    
    //todo gef's way of getting the actual ID is broken
    // assume mesh data list is a parallel list, (bad)
    
    auto meshDataIter = meshDataList.begin();
    for(auto skeletonIter = skeletons.begin(); skeletonIter != skeletons.end(); ++skeletonIter, ++ meshDataIter)
    {
       // gef::StringId uniqueID = reinterpret_cast<unsigned>(skeleton);  // NOLINT
        gef::StringId uniqueID = meshDataIter->name_id;
        
        auto wrappedSkeleton = SkeletonWrapper::Create(**skeletonIter, uniqueID);
        
        skeletons_.emplace(uniqueID, std::move(wrappedSkeleton));
    }

    return Result::OK();

    // todo use this code 
    for (auto & meshData : meshDataList)
    {
        // const auto skeleton = scene.FindSkeleton(meshData);
        //
        // if(skeleton == nullptr)
        //     return Result::Error("Skeleton " + std::to_string(meshData.name_id) + "could not be found");
        //
        // auto wrappedSkeleton = GefSkeletonWrapper::Create(*skeleton,);
        // skeletons_.emplace(meshData.name_id, std::move(wrappedSkeleton));
    }
	
    return Result::OK();
}

AnimationSystem::SkeletonWrapper const* AnimationSystem::SkeletonLoader::GetSkeleton(std::string const& name) const
{
    return GetSkeleton(gef::GetStringId(name));
}

AnimationSystem::SkeletonWrapper const* AnimationSystem::SkeletonLoader::GetSkeleton(gef::StringId id) const
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

AnimationSystem::Result AnimationSystem::SingleSkeletonLoader::LoadSkeleton(StringId const id, gef::Scene const& scene, gef::Platform & platform)
{
    const auto & skeletons = scene.skeletons;
    
    if(skeletons.empty())
        return Result::Error("Scene contained no skeletons");

    if(skeletons.size() > 1)
        return Result::Error("Multiple skeletons per scene is not supported");

    const auto skeletonFromFile = skeletons.front();
    skeleton_ = SkeletonWrapper::Create(*skeletonFromFile, id);

    return Result::OK();
}
