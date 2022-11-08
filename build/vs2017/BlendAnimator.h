#pragma once

#include "AnimationMath.h"
#include "PureResult.h"
#include "AnimatorWrapper.h"

using std::string;
namespace AnimationSystem
{
    struct BlendLerpValue
    {
        BlendLerpValue(float const v)
            :Value(std::clamp(v, 0.f, 1.f)) {  }

        static BlendLerpValue SetFromInverseLerp(float const begin, float const end, float const v)
        {return {AnimationMath::InverseLerp(begin, end, v)};}
        
        static BlendLerpValue SetBeginValue() {return {0};}
        static BlendLerpValue SetEndValue() {return {1};}

        operator float() const { return Value; }
        const float Value;
    };

    struct Transition
    {
        Transition(float const beginTime, float const duration)
            : begin_time_(beginTime)
            , duration_(duration)
        {}

        void AccumulateTime(float const time){accumulated_time_ += time;}
        [[nodiscard]] BlendLerpValue LerpValue() const {return BlendLerpValue::SetFromInverseLerp(begin_time_, EndTime(), CurrentTime());}
        [[nodiscard]] bool InTransition() const {return (CurrentTime() < EndTime()); }
    private:
        float accumulated_time_ = 0;
        [[nodiscard]] float CurrentTime() const {return begin_time_ + accumulated_time_;}
        const float begin_time_;
        const float duration_;
        [[nodiscard]] float constexpr EndTime()const {return begin_time_ + duration_;}
    };
    
    class BlendAnimator
    {
    public:
        explicit BlendAnimator();
        void Init(gef::SkeletonPose const & bindPose);
        
        [[nodiscard]] AnimatorWrapper & ActiveAnimator() const;

        gef::SkeletonPose UpdateAnimationSimple(float const frameTime, gef::SkeletonPose const& skeletonPose);

        void UpdateTransition(float frameTime);

        gef::SkeletonPose UpdateAnimationBlended(float frameTime, gef::SkeletonPose const& skeletonPose);

        gef::SkeletonPose UpdateAnimation(float const frameTime, gef::SkeletonPose const &skeletonPose);

        PureResult SetAnimation(Animation const& animation, float transitionTime)
        {
            return ActiveAnimator().SetAnimation(animation);
        }


    private:
        [[nodiscard]] AnimatorWrapper & AlternateAnimator() const;

        enum class Controller { Left, Right };


        Controller current_animator_;
        
        std::optional<Transition> transition_;
        [[nodiscard]] bool InTransition() const;

        /**
         * \brief Store exactly two animators
         */
        struct AnimationControllerPair
        {
            [[nodiscard]] AnimatorWrapper& Get(Controller const controller) const;
        private:
            std::pair<AnimatorWrapper, AnimatorWrapper> controllers_;
            std::map<Controller, std::function<AnimatorWrapper&()>> map_ {
                {Controller::Left, [this] () -> AnimatorWrapper& {return controllers_.first;}},
                {Controller::Right, [this] () -> AnimatorWrapper& {return controllers_.second;}},
            };
        } controllers_;

        static Controller ControllerOther(Controller const current);
    };
}
