#pragma once
#include "Animation.h"
#include "AnimatorConfig.h"
#include "SkinnedMeshContainer.h"

namespace AnimationSystem
{
    class AnimatorWrapper : IAnimatorConfig
    {
    public:
        void Init(gef::SkeletonPose const& skeletonPose) { animator_.Init(skeletonPose); }
        [[nodiscard]]gef::SkeletonPose UpdateAnimation(const float frameTime, gef::SkeletonPose const &bindPose);
        PureResult SetAnimation(const Animation& animation);
        
        void SetAnimationTime(const float animationTime) override { animator_.set_anim_time(animationTime); }
        void SetPlaybackSpeed(const float playbackSpeed) override { animator_.set_playback_speed(playbackSpeed); }
        void SetLooping(const bool isLooping) override { animator_.set_looping(isLooping); }

    private:
        PureResult ApplyConfigs(Animation const& animation);
        MotionClipPlayer animator_;
    };
}