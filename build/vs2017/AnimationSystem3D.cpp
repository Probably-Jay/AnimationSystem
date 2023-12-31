#include "AnimationSystem3D.h"

#include "AnimatedObject.h"
#include "MeshLoader.h"

#include <optional>

using namespace AnimationSystem;

AnimationSystem3D::AnimationSystem3D(gef::Platform& platform_)
    : animated_objects_factory_(new AnimatedObjectFactory())
    , platform_(platform_)
{

}

unique_ptr<AnimationSystem3D> AnimationSystem3D::Create(gef::Platform& platform_)
{
    const auto system = new AnimationSystem3D(platform_);
    return unique_ptr<AnimationSystem3D>(system);
}

ValueResult<std::shared_ptr<IAnimatedObject>> AnimationSystem3D::CreateAnimatedObject(
    string const& objectNameId, string const& filePath) const
{
    return animated_objects_factory_->CreateAnimatedObject(objectNameId, filePath, platform_);
}

PureResult AnimationSystem3D::CreateAnimationFor(IAnimatedObject const& readonlyAnimObject, string const& animationName,
                                                 string const& fileName, string const& nameWithinFile,
                                                 AnimationClip::OptionalConfigOnSetAnimationDelegate && configurationDelegate) const
{
    auto animatedObjectResult = animated_objects_factory_->FindObject(readonlyAnimObject);
    if(animatedObjectResult.IsError())
        return animatedObjectResult.ToPureResult();

    auto & animatedObject = animatedObjectResult.Take().get();
    
    PureResult createAnimationResult = animatedObject.Animator().CreateAnimation(animationName, fileName, nameWithinFile, std::move(configurationDelegate));
    return createAnimationResult;
}



