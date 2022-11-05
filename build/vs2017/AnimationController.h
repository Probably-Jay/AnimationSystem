#pragma once
#include <functional>

#include "AnimatorConfig.h"
#include "AnimatorWrapper.h"
#include "IAnimationContainer.h"
#include "SkinnedMeshWrapper.h"

namespace AnimationSystem
{

    class IAnimator
    {
    public:
        virtual ~IAnimator() = default;
        virtual PureResult SetAnimation(std::string animationName) = 0;
        virtual PureResult SetAnimation(StringId animationId) = 0;

    };


    // typedef OwningProtectedWrapper<AnimationController> AnimationControllerWrapper;
   class AnimationController : public IAnimator
   {
   public:
       AnimationController(StringId id, gef::Platform const& platform);
       //  Result SetCurrentAnimation(StringId id);

       void Animate() { throw; }
       void Init(gef::SkinnedMeshInstance const& skinnedMesh);
        
       PureResult CreateAnimation(const string& animationName, const std::string& filePath,
                                  const std::string& nameWithinFile, const std::function<void(AnimatorConfig)> configDelegate);
       PureResult SetAnimation(std::string animationName) override;
       PureResult SetAnimation(StringId animationId) override;

       PureResult UpdateAnimation(float frameTime, gef::SkinnedMeshInstance& skinnedMesh);

   private:
       PureResult SetAnimation(Animation& animation);
       std::optional<std::reference_wrapper<Animation>> current_animation_;
        
       std::unique_ptr<IAnimationContainer> animations_;
       std::unique_ptr<MotionClipPlayer> animator_;

       //  AnimationWrapper * CurrentAnimation() const {return animations_.GetAnimation(currentAnimationId);}
   };
}
