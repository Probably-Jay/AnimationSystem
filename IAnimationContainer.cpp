#include "IAnimationContainer.h"

#include "graphics/scene.h"

AnimationSystem::AnimationContainer::AnimationContainer(const gef::Platform& platform)
    :platform_(platform)
{
}

AnimationSystem::PureResult AnimationSystem::AnimationContainer::LoadAnimations(const string& animationName, const std::string& filepath,
    const std::string& nameWithinFile, std::optional<std::function<void(IAnimatorConfig&)> const> const configDelegate)
{
    auto animationScene = std::make_unique<gef::Scene>();
	
    if (!animationScene->ReadSceneFromFile(platform_, filepath.c_str()))
        return PureResult::Error(ERROR_TAG+"Could not load animation from scene file at: " + filepath);
    
    const auto animationIter =
        nameWithinFile.empty() // if no name specified, take first
            ? animationScene->animations.begin()
            : animationScene->animations.find(gef::GetStringId(nameWithinFile));

    if (animationIter == animationScene->animations.end())
        return PureResult::Error(ERROR_TAG+"Animation with name '" + nameWithinFile + "' could not be found");

    const StringId animationId = stringIdTable_.Add(animationName);

    auto & gefAnimation = *animationIter->second;
    auto animation = std::make_unique<Animation>(std::move(animationScene), gefAnimation, animationId, configDelegate);
    
    animations_.emplace(animationId, std::move(animation));

    return PureResult::OK();
}

AnimationSystem::ValueResult<std::reference_wrapper<AnimationSystem::Animation>> AnimationSystem::AnimationContainer::GetAnimation(
    const StringId id) const
{
    const auto iter = animations_.find(id);
    if(iter == animations_.end())
        return ValueResult<std::reference_wrapper<Animation>>::Error(ERROR_TAG+"Could not find animation with id: " + std::to_string(id));
    
    return ValueResult<std::reference_wrapper<Animation>>::OK(*iter->second);
}

std::optional<string> AnimationSystem::AnimationContainer::GetAnimationName(StringId const id) const
{
    if(string result; stringIdTable_.Find(id, result))
        return result;
    return {};
}
