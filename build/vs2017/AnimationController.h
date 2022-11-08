#pragma once
#include <functional>

#include "AnimatorConfig.h"
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
        virtual std::optional<string> CurrentAnimationName() = 0;
    };

    class AnimationController : public IAnimator
   {
   public:
       AnimationController(gef::Platform const& platform);

       void Init(gef::SkinnedMeshInstance const& skinnedMesh);
        
       PureResult CreateAnimation(const string& animationName, const std::string& filePath,
                                  const std::string& nameWithinFile, std::optional<std::function<void(IAnimatorConfig)> const> const configDelegate);
       
       PureResult SetAnimation(std::string animationName) override;
       PureResult SetAnimation(StringId animationId) override;

       PureResult UpdateAnimation(float frameTime, gef::SkinnedMeshInstance& skinnedMesh);

       std::optional<string> CurrentAnimationName() override
       {
           return animations_->GetAnimationName(current_animation_->get().ID());
       }
   private:
       PureResult SetAnimation(Animation& animation);

       std::optional<std::reference_wrapper<Animation>> current_animation_;
        
       std::unique_ptr<IAnimationContainer> animations_{};
       std::unique_ptr<class AnimatorWrapper> animator_{};
   };

   class AnimatorWrapper : IAnimatorConfig
   {
   public:
       void UpdateAnimation(float frameTime, gef::SkinnedMeshInstance& skinnedMesh);
       PureResult SetAnimation(const Animation& animation);
       void SetAnimationTime(const float animationTime) override {animator_.set_anim_time(animationTime);}
       void SetPlaybackSpeed(const float playbackSpeed) override {animator_.set_playback_speed(playbackSpeed); }
       void SetLooping(const bool isLooping) override {animator_.set_looping(isLooping); }
       MotionClipPlayer & Animator() {return animator_;}
   private:
       MotionClipPlayer animator_;
   };
}
