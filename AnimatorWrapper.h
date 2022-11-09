#pragma once
#include "AnimationClip.h"
#include "AnimationMath.h"
#include "AnimatorConfig.h"
#include "motion_clip_player.h"
#include "PureResult.h"


namespace AnimationSystem
{
    class AnimatorWrapper : public IAnimatorConfig
    {
    public:
        void Init(gef::SkeletonPose const& skeletonPose) { animator_.Init(skeletonPose); }
        [[nodiscard]]gef::SkeletonPose UpdateAnimation(const float frameTime, gef::SkeletonPose const &bindPose);
        PureResult SetAnimation(const AnimationClip& animation);
        PureResult SetAnimation(AnimatorWrapper const& animator);

        void SetAnimationTime(const float animationTime) override { animator_.set_anim_time(animationTime); }
        void SetPlaybackSpeed(const float playbackSpeed) override { animator_.set_playback_speed(playbackSpeed); }
        void SetLooping(const bool isLooping) override { animator_.set_looping(isLooping); }


        void SetAnimationTimePercent(float const percent)
        {
            if(!current_animation_.has_value())
                return;
            auto const animationDuration = GetCurrentClipDuration();
            const auto percentClamped = std::clamp(percent,0.f,1.f);
            
            auto const animationTime = std::lerp(0.f, animationDuration, percentClamped );
            SetAnimationTime(animationTime);
        }


        [[nodiscard]] float GetCurrentAnimationProgress() const override
        {
            if(!current_animation_.has_value())
                return 0;
            auto const animationDuration = GetCurrentClipDuration();
            auto const currentAnimationTime = animator_.anim_time();
            return AnimationMath::InverseLerp(0,animationDuration, currentAnimationTime);
        }

        [[nodiscard]] float GetCurrentClipDuration() const override
        {
            if(!current_animation_.has_value())
                return 0;
            return CurrentAnimation().GetAnimation().duration();
        }

        [[nodiscard]] float GetDurationScalingFactor(class IAnimatorConfig const& otherAnimator) const override;


    private:
        [[nodiscard]] AnimationClip const& CurrentAnimation() const {return current_animation_->get();}

        PureResult ApplyConfigs(AnimationClip const& animation);

    private:
        MotionClipPlayer animator_;
        std::optional<std::reference_wrapper<AnimationClip const>> current_animation_;
    };
}
