#pragma once
#include "motion_clip_player.h"

/**
 * \brief Restricts the MotionClipPlayer interface
 */
class IAnimatorConfig 
{
public:
    virtual ~IAnimatorConfig() = default;
    virtual void SetAnimationTime(float animationTime) =0;
    virtual void SetPlaybackSpeed(float playbackSpeed) =0;
    virtual void SetLooping(bool isLooping) =0;
};

// class IAnimator
// {
// public:
//     virtual ~IAnimator() = default;
//     virtual void SetAnimationTime(float animationTime) =0;
//     virtual void SetPlaybackSpeed(float playbackSpeed) =0;
//     virtual void SetLooping(bool isLooping) =0;
// };