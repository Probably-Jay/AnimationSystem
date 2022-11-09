#pragma once
#include <functional>

#include "AnimatorConfig.h"
#include "BlendAnimator.h"
#include "IAnimationContainer.h"

namespace AnimationSystem
{
    class IAnimationController
    {
    public:
        virtual ~IAnimationController() = default;
        virtual PureResult SetAnimation(std::string animationName, float transitionTime = 0, Animation::OptionalConfigOnTransitionAnimationDelegate blendDelegate ={}) = 0;
        virtual PureResult SetAnimation(StringId animationId, float transitionTime = 0, Animation::OptionalConfigOnTransitionAnimationDelegate ={}) = 0;
        virtual std::optional<string> CurrentAnimationName() = 0;
    };

    /**
     * \brief Class containing the animator and all animations of an object
     */
    class AnimationController : public IAnimationController
    {
    public:
        AnimationController(gef::Platform const& platform);

        void Init(gef::SkinnedMeshInstance const& skinnedMesh);

        PureResult CreateAnimation(const string &animationName, const std::string &filePath,
                                   const std::string &nameWithinFile,
                                   Animation::OptionalConfigOnSetAnimationDelegate configDelegate);

        PureResult SetAnimation(std::string animationName, float transitionTime, Animation::OptionalConfigOnTransitionAnimationDelegate transitionDelegate) override;
        PureResult SetAnimation(StringId animationId, float transitionTime, Animation::OptionalConfigOnTransitionAnimationDelegate transitionDelegate ) override;

        ValueResult<gef::SkeletonPose> UpdateAnimation(const float frameTime, gef::SkeletonPose const &skeletonPose);

        std::optional<string> CurrentAnimationName() override
        {
            if(!current_animation_name_.has_value())
                return {};
            return animations_->GetAnimationName(current_animation_name_.value());
        }

    private:
        PureResult SetAnimation(Animation const &animation, float const transitionTime, Animation::OptionalConfigOnTransitionAnimationDelegate
                                transitionDelegate);

        std::optional<StringId> current_animation_name_;

        std::unique_ptr<AnimationContainer> animations_{};
        std::unique_ptr<BlendAnimator> animator_{};
    };

   
}
