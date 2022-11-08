#include "AnimatedObjectFactory.h"


AnimationSystem::ValueResult<std::shared_ptr<AnimationSystem::IAnimatedObject>>
AnimationSystem::AnimatedObjectFactory::CreateAnimatedObject(
    string const& objectNameId, string const& filePath, gef::Platform& platform)
{
    const auto idIsUniqueResult = GefExtensions::TryAddNew(id_table_, objectNameId);
    if(idIsUniqueResult.IsError())
        return ValueResult<std::shared_ptr<IAnimatedObject>>::Error(idIsUniqueResult.Error());

    // get the ID
    const auto stringId = idIsUniqueResult.Get();
    
    auto modelScene = std::make_unique<gef::Scene>();
    modelScene->ReadSceneFromFile(platform, filePath.c_str());

    auto animatedObject = std::shared_ptr<AnimatedObject>{new AnimatedObject{platform, stringId}};

    // Load the object
    if(const auto result = animatedObject->CreateObjectsFromScene(platform, std::move(modelScene)); result.IsError())
        return ValueResult<std::shared_ptr<IAnimatedObject>>::Error(result.Error().value());

    map_.emplace(stringId, animatedObject);

    return ValueResult<std::shared_ptr<IAnimatedObject>>::OK(animatedObject);
}

AnimationSystem::ValueResult<std::reference_wrapper<AnimationSystem::AnimatedObject>> AnimationSystem::AnimatedObjectFactory::FindObject(IAnimatedObject const& animatedObject)
{
    using value_result = ValueResult<std::reference_wrapper<AnimatedObject>>;
    const auto iter = map_.find(animatedObject.ID());
    if(iter == map_.end())
        return value_result::Error(ERROR_TAG+"Could not find animated object");

    return value_result::OK(*iter->second);
}  