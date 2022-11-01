#pragma once
#include "motion_clip_player.h"

/**
 * \brief Restricts the MotionClipPlayer interface
 */
class AnimatorConfig 
{
public:
    AnimatorConfig(MotionClipPlayer& motionClipPlayer): motion_clip_player_(motionClipPlayer) {  }
    void SetAnimationTime(const float animationTime) {motion_clip_player_.set_anim_time(animationTime);}
    void SetPlaybackSpeed(const float playbackSpeed) {motion_clip_player_.set_playback_speed(playbackSpeed); }
    void SetLooping(const bool isLooping) {motion_clip_player_.set_looping(isLooping); }
private:
    MotionClipPlayer& motion_clip_player_;
};
