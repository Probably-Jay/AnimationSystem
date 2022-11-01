#include "IAnimationContainer.h"

#include "AnimationWrapper.h"
#include "graphics/scene.h"

AnimationSystem::AnimationContainer::AnimationContainer(const gef::Platform& platform)
    :platform_(platform)
{
}

AnimationSystem::CreateEntityResult AnimationSystem::AnimationContainer::LoadAnimations(const string& nameId, const std::string& filepath, const std::string& nameWithinFile)
{
    auto animationScene = gef::Scene{};
	
    if (!animationScene.ReadSceneFromFile(platform_, filepath.c_str()))
        return CreateEntityResult::Error("Could not load animation from scene file");
    
    const auto animationIter =
        nameWithinFile.empty() // if no name specified, take first
            ? animationScene.animations.begin()
            : animationScene.animations.find(gef::GetStringId(nameWithinFile));

    if (animationIter == animationScene.animations.end())
        return CreateEntityResult::Error("Animation with name " + nameWithinFile + "could not be found");

    const StringId animationId = string_id_table_.Add(nameId);
    
    auto animation = AnimationWrapper::Create(animationId, std::move(*animationIter->second));
    animations_.emplace(animationId, std::move(animation));

    return CreateEntityResult::OK(animationId);
}
