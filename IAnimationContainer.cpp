#include "IAnimationContainer.h"

#include "AnimationWrapper.h"
#include "graphics/scene.h"

AnimationSystem::AnimationContainer::AnimationContainer(const gef::Platform& platform)
    :platform_(platform)
{
}

AnimationSystem::CreateEntityResult AnimationSystem::AnimationContainer::LoadAnimations(
    const string& filepath, string animationName)
{
    // from lab
    
    auto animationScene = gef::Scene{};
	
    if (!animationScene.ReadSceneFromFile(platform_, filepath.c_str()))
        return CreateEntityResult::Error("Could not load animation from scene file");
    
    const auto animationIter =
        animationName.empty() // if no name specified, take first
            ? animationScene.animations.begin()
            : animationScene.animations.find(gef::GetStringId(animationName));

    if (animationIter == animationScene.animations.end())
        return CreateEntityResult::Error("Animation with name " + animationName + "could not be found");

    // create a UID todo make this better
    const StringId animationId = gef::GetStringId(filepath) ^ animationIter->first;
    
    auto animation = AnimationWrapper::Create(animationId, std::move(*animationIter->second));

    StringId id = animation->ID();
    animations_.emplace(id, std::move(animation));

    return CreateEntityResult::OK(id);
}
