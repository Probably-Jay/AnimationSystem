#include "SkeletonLoader.h"

AnimationSystem::SkeletonLoader::SkeletonLoader(gef::Platform& platform_)
    : platform_(platform_)
{
}

Result AnimationSystem::SkeletonLoader::LoadSkeletonScene(gef::Scene& scene)
{
    
    const auto & skeletons = scene.skeletons;
	
    if(skeletons.empty())
        return Result::Error("Scene contained no meshes");
    //
    // for (auto & meshData : meshDataList)
    // {
    //     auto mesh = std::unique_ptr<gef::Mesh>(scene.CreateMesh(platform_, meshData));
    //     if(mesh == nullptr)
    //         return Result::Error("Mesh " + std::to_string(meshData.name_id) + "could not be created");
    //
    //     auto wrappedMesh = GefMeshWrapper::Create(std::move(mesh));
    //     meshes_.emplace(meshData.name_id, std::move(wrappedMesh));
    // }
	
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
