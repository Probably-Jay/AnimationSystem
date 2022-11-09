#include "IAnimationContainer.h"

#include "graphics/scene.h"

AnimationSystem::AnimationContainer::AnimationContainer(const gef::Platform& platform)
    :platform_(platform)
{
}


AnimationSystem::PureResult AnimationSystem::AnimationContainer::LoadAnimations(const string& animationName, const std::string& filepath,
    const std::string& nameWithinFile, Animation::OptionalConfigOnSetAnimationDelegate configDelegate)
{
    const auto result = GefExtensions::TryAddNew(stringIdTable_, animationName);
    if(result.IsError())
        return result.ToPureResult();
    
    const StringId animationId = result.Get();

    // Create the animation
    auto createAnimResult = CreateAnimation(filepath, nameWithinFile, std::move(configDelegate), animationId);
    
    if(createAnimResult.IsError())
        return createAnimResult.ToPureResult();
    
    animations_.emplace(animationId, createAnimResult.Take());
    return PureResult::OK();
}

AnimationSystem::ValueResult<std::unique_ptr<AnimationSystem::Animation>>
AnimationSystem::AnimationContainer::CreateAnimation(const std::string &filepath, const std::string &nameWithinFile,
                                                     Animation::OptionalConfigOnSetAnimationDelegate configDelegate, const StringId animationId) const
{
    auto animationScene = std::make_unique<gef::Scene>();
	
    if (!animationScene->ReadSceneFromFile(platform_, filepath.c_str()))
        return ValueResult<std::unique_ptr<Animation>>::Error(ERROR_TAG+"Could not load animation from scene file at: " + filepath);

    const auto animationIter =
        nameWithinFile.empty() // if no name specified, take the first animation in the file
            ? animationScene->animations.begin()
            : animationScene->animations.find(gef::GetStringId(nameWithinFile));

    if (animationIter == animationScene->animations.end())
        return ValueResult<std::unique_ptr<Animation>>::Error(ERROR_TAG+"Animation with name '" + nameWithinFile + "' could not be found");

    auto & gefAnimation = *animationIter->second;
    auto animation = std::make_unique<Animation>(std::move(animationScene), gefAnimation, animationId,std::move(configDelegate));
    
    return ValueResult<std::unique_ptr<Animation>>::OK(std::move(animation));
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
