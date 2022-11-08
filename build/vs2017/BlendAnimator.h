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
        explicit Transition(float const duration)
            : duration_(duration)
        {}

        void AccumulateTime(float const time){accumulated_time_ += time;}
        [[nodiscard]] BlendLerpValue LerpValue() const {return BlendLerpValue::SetFromInverseLerp(0, duration_, accumulated_time_);}
        [[nodiscard]] bool InTransition() const {return (accumulated_time_ < duration_); }
    private:
        float accumulated_time_ = 0;
        const float duration_;
    };
    
    class BlendAnimator
    {
    public:
        explicit BlendAnimator();
        void Init(gef::SkeletonPose const & bindPose);
        
        [[nodiscard]] AnimatorWrapper & ActiveAnimator() const;

        gef::SkeletonPose UpdateAnimationSimple(float frameTime, gef::SkeletonPose const& skeletonPose);

        void UpdateTransition(float frameTime);

        gef::SkeletonPose UpdateAnimationBlended(float frameTime, gef::SkeletonPose const& skeletonPose);

        gef::SkeletonPose UpdateAnimation(float frameTime, gef::SkeletonPose const &skeletonPose);

        PureResult SetAnimationSimple(Animation const& animation)
        {
            return ActiveAnimator().SetAnimation(animation);
        }
        

        PureResult SetAnimation(Animation const& animation, float transitionTime)
        {
            if(transitionTime <= 0)
                return SetAnimationSimple(animation);

            if(auto setAltResult = AlternateAnimator().SetAnimation(ActiveAnimator()); setAltResult.IsError())
                return setAltResult;
            
            if(auto setActiveResult = ActiveAnimator().SetAnimation(animation); setActiveResult.IsError())
               return setActiveResult;

            transition_ = std::make_unique<Transition>(transitionTime);
            return PureResult::OK();
        }


    private:
        [[nodiscard]] AnimatorWrapper & AlternateAnimator() const;

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
