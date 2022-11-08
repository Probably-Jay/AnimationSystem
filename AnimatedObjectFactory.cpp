#include "AnimatedObjectFactory.h"


AnimationSystem::CreateAnimatedObjectResult AnimationSystem::AnimatedObjectFactory::CreateAnimatedObject(
    string const & objectNameId, string const& filePath, gef::Platform & platform)
{
    auto modelScene = std::make_unique<gef::Scene>();
    modelScene->ReadSceneFromFile(platform, filePath.c_str());

    const auto stringId = id_table_.Add(objectNameId);
    
    auto animatedObject = std::shared_ptr<AnimatedObject>{new AnimatedObject{platform, stringId}};

    // Load the object
    if(const auto result = animatedObject->CreateObjectsFromScene(platform, std::move(modelScene)); result.IsError())
        return CreateAnimatedObjectResult::Error(result.Error().value());

    map_.emplace(stringId, animatedObject);

    return CreateAnimatedObjectResult::OK(animatedObject);
}

AnimationSystem::ValueResult<std::reference_wrapper<AnimationSystem::AnimatedObject>> AnimationSystem::AnimatedObjectFactory::FindObject(IAnimatedObject const& animatedObject)
{
    using value_result = ValueResult<std::reference_wrapper<AnimatedObject>>;
    const auto iter = map_.find(animatedObject.ID());
    if(iter == map_.end())
        return value_result::Error(ERROR_TAG+"Could not find animated object");

    return value_result::OK(*iter->second);
}  