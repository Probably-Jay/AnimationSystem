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
        explicit Transition(float const duration, Animation::OptionalConfigOnTransitionAnimationDelegate transitionDelegate)
            : duration_(duration), transition_delegate_(std::move(transitionDelegate))
        {}

        void AccumulateTime(float const time){accumulated_time_ += time;}
        [[nodiscard]] BlendLerpValue LerpValue() const {return BlendLerpValue::SetFromInverseLerp(0, duration_, accumulated_time_);}
        [[nodiscard]] bool InTransition() const {return (LerpValue() < 1); }

      //  Animation::OptionalConfigOnTransitionAnimationDelegate const & GetTransitionDelegate() const {return transition_delegate_;}
        void ApplyDelegate(AnimatorWrapper & activeAnimator) const noexcept(false)
        {
            if(!transition_delegate_.has_value())
                return;
            
            transition_delegate_.value()(activeAnimator, LerpValue());
        }
    private:
        float accumulated_time_ = 0;
        const float duration_;
        Animation::OptionalConfigOnTransitionAnimationDelegate transition_delegate_;
    };
    
    class BlendAnimator
    {
    public:
        explicit BlendAnimator();
        void Init(gef::SkeletonPose const & bindPose);
        

        gef::SkeletonPose UpdateAnimation(float frameTime, gef::SkeletonPose const &skeletonPose);

        PureResult SetAnimation(Animation const& animation, float transitionTime, Animation::OptionalConfigOnTransitionAnimationDelegate transitionDelegate);


    private:
        [[nodiscard]] AnimatorWrapper & ActiveAnimator() const;
        [[nodiscard]] AnimatorWrapper & AlternateAnimator() const;

        [[nodiscard]]gef::SkeletonPose UpdateAnimationSimple(float frameTime, gef::SkeletonPose const& skeletonPose);

        [[nodiscard]] ValueResult<gef::SkeletonPose> UpdateAnimationBlended(
            float frameTime, gef::SkeletonPose const &skeletonPose);
        PureResult ApplyTransitionDelegate();

        [[nodiscard]] PureResult SetAnimationSimple(Animation const& animation);

        [[nodiscard]] PureResult UpdateTransitionAndApplyTransitionDelegate(float frameTime);

        void SyncAnimators();

        enum class Controller { Left, Right };


        Controller current_animator_;
        
        std::unique_ptr<Transition> transition_{};
        [[nodiscard]] bool InTransition() const;

        /**
         * \brief Store exactly two animators
         */
        struct AnimationControllerPair
        {
            [[nodiscard]] AnimatorWrapper& Get(Controller controller) const;
        private:
            std::pair<AnimatorWrapper, AnimatorWrapper> controllers_;
            std::map<Controller, std::function<AnimatorWrapper&()>> map_ {
                {Controller::Left, [this] () -> AnimatorWrapper& {return controllers_.first;}},
                {Controller::Right, [this] () -> AnimatorWrapper& {return controllers_.second;}},
            };
        } controllers_;

        static Controller ControllerOther(Controller current);
    };
}
