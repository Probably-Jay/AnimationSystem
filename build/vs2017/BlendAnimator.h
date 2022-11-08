#pragma once

#include "PureResult.h"
#include "AnimatorWrapper.h"

using std::string;
namespace AnimationSystem
{
    class BlendAnimator
    {
    public:
        explicit BlendAnimator()
            : current_(Controller::Left)
        {
        }
        void Init(gef::SkeletonPose const & bindPose);
        
        [[nodiscard]] AnimatorWrapper & ActiveAnimator() const {return controllers_.Get(current_);}

    private:
        [[nodiscard]] AnimatorWrapper & AlternateAnimator() const {return controllers_.Get(ControllerOther(current_));}
        
        enum class Controller { Left, Right };
        
        Controller current_;

        /**
         * \brief Store exactly two animators
         */
        struct AnimationControllerPair
        {
            [[nodiscard]] AnimatorWrapper& Get(const Controller controller) const {return map_.at(controller)();}
        private:
            std::pair<AnimatorWrapper, AnimatorWrapper> controllers_;
            std::map<Controller, std::function<AnimatorWrapper&()>> map_ {
                {Controller::Left, [this] () -> AnimatorWrapper& {return controllers_.first;}},
                {Controller::Right, [this] () -> AnimatorWrapper& {return controllers_.second;}},
            };
        } controllers_;

        static Controller ControllerOther(const Controller current)
        {
            switch (current)
            {
            case Controller::Left: return Controller::Right;
            case Controller::Right: return Controller::Left;
            }
            throw;
        }
    };
}
