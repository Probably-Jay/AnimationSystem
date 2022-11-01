#include "AnimatedObjectContainer.h"

AnimationSystem::CreateEntityResult AnimationSystem::AnimatedObjectContainer::CreateAnimatedObject(
    string const & objectNameId, string const& filePath, gef::Platform & platform)
{
    auto modelScene = std::make_unique<gef::Scene>();
    modelScene->ReadSceneFromFile(platform, filePath.c_str());

    const auto stringId = idTable.Add(objectNameId);
    
    auto animatedObject = AnimatedObjectWrapper::Create(stringId, stringId);
    const auto result = animatedObject->Item().Create(platform,  std::move(modelScene));

    if(result.IsError())
        return CreateEntityResult::Error(result);

    map_.emplace(stringId, std::move(animatedObject));

    return CreateEntityResult::OK(stringId);
}  