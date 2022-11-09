#pragma once

namespace AnimationSystem
{
    class AnimationClip;
}

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
    [[nodiscard]] virtual float GetCurrentClipDuration() const = 0;
    [[nodiscard]] virtual float GetCurrentAnimationProgress() const = 0;
    [[nodiscard]] virtual float GetDurationScalingFactor(IAnimatorConfig const & otherAnimator) const = 0;
    
};

