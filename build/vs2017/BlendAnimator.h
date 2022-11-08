#pragma once
#include "AnimationController.h"

namespace AnimationSystem
{
    using gef::StringId;
    class BlendAnimator
    {
    public:
        explicit BlendAnimator(gef::Platform const & platform)
            : current_(Controller::Left)
            , controllers_(platform)

        {
        }

        PureResult SetAnimation(string const &name) {return Current().SetAnimation(name);}        
        PureResult SetAnimation(StringId const id) {return Current().SetAnimation(id);}        

    private:
        [[nodiscard]] IAnimator & Current() const {return controllers_.Get(current_);}
        [[nodiscard]] IAnimator & Alternate() const {return controllers_.Get(ControllerOther(current_));}
        
        enum class Controller { Left, Right };
        
        Controller current_;
        
        struct AnimationControllerPair
        {
            using animator = MotionClipPlayer;
            
            AnimationControllerPair(gef::Platform const & platform) : controllers_(std::pair{animator{platform}, animator{platform}}){  }

            [[nodiscard]] animator& Get(const Controller controller) const {return map_.at(controller)();}
        private:
            std::pair<animator, animator> controllers_;
            std::map<Controller, std::function<animator&()>> map_ {
                {Controller::Left, [this] () -> animator& {return controllers_.first;}},
                {Controller::Right, [this] () -> animator& {return controllers_.second;}},
            };
        } controllers_;

        static Controller ControllerOther(const Controller current)
        {
            switch (current) {
            case Controller::Left: return Controller::Right;
            case Controller::Right: return Controller::Left;
            default: throw;
            }
        }
    };
}
