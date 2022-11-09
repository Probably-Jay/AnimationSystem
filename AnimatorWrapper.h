#pragma once
#include "Animation.h"
#include "AnimationMath.h"
#include "AnimatorConfig.h"
#include "SkinnedMeshContainer.h"

namespace AnimationSystem
{
    class AnimatorWrapper : public IAnimatorConfig
    {
    public:
        void Init(gef::SkeletonPose const& skeletonPose) { animator_.Init(skeletonPose); }
        [[nodiscard]]gef::SkeletonPose UpdateAnimation(const float frameTime, gef::SkeletonPose const &bindPose);
        PureResult SetAnimation(const Animation& animation);
        PureResult SetAnimation(AnimatorWrapper const& animator);

        void SetAnimationTime(const float animationTime) override { animator_.set_anim_time(animationTime); }
        void SetPlaybackSpeed(const float playbackSpeed) override { animator_.set_playback_speed(playbackSpeed); }
        void SetLooping(const bool isLooping) override { animator_.set_looping(isLooping); }

        void SetAnimationTimePercent(float const percent)
        {
            if(!current_animation_.has_value())
                return;
            auto const animationDuration = CurrentAnimationDuration();
            const auto percentClamped = std::clamp(percent,0.f,1.f);
            
            auto const animationTime = std::lerp(0.f, animationDuration, percentClamped );
            SetAnimationTime(animationTime);
        }


        [[nodiscard]] std::optional<float> GetPercentThroughAnimation() const
        {
            if(!current_animation_.has_value())
                return {};
            auto const animationDuration = CurrentAnimationDuration();
            auto const currentAnimationTime = animator_.anim_time();
            return AnimationMath::InverseLerp(0,animationDuration, currentAnimationTime);
        }
    private:
        [[nodiscard]] float CurrentAnimationDuration() const { return CurrentAnimation().GetAnimation().duration(); }
        PureResult ApplyConfigs(Animation const& animation);
        MotionClipPlayer animator_;
        Animation const&CurrentAnimation() const {return current_animation_->get();}
        std::optional<std::reference_wrapper<Animation const>> current_animation_;
    };
}