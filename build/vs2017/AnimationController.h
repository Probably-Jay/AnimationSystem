#pragma once
#include <functional>

#include "AnimatorConfig.h"
#include "BlendAnimator.h"
#include "IAnimationContainer.h"
#include "SkinnedMeshWrapper.h"

namespace AnimationSystem
{
    class IAnimator
    {
    public:
        virtual ~IAnimator() = default;
        virtual PureResult SetAnimation(std::string animationName) = 0;
        virtual PureResult SetAnimation(StringId animationId) = 0;
        virtual std::optional<string> CurrentAnimationName() = 0;
    };

    /**
     * \brief Class containing the animator and all animations of an object
     */
    class AnimationController : public IAnimator
    {
    public:
        AnimationController(gef::Platform const& platform);

        void Init(gef::SkinnedMeshInstance const& skinnedMesh);

        PureResult CreateAnimation(const string& animationName, const std::string& filePath,
                                   const std::string& nameWithinFile,
                                   std::optional<std::function<void(IAnimatorConfig&)> const> configDelegate);

        PureResult SetAnimation(std::string animationName) override;
        PureResult SetAnimation(StringId animationId) override;

        PureResult UpdateAnimation(float frameTime, gef::SkinnedMeshInstance& skinnedMesh);

        std::optional<string> CurrentAnimationName() override
        {
            if(!current_animation_name_.has_value())
                return {};
            return animations_->GetAnimationName(current_animation_name_.value());
        }

    private:
        PureResult SetAnimation(Animation& animation);

        std::optional<StringId> current_animation_name_;

        std::unique_ptr<IAnimationContainer> animations_{};
        std::unique_ptr<BlendAnimator> animator_{};
    };

   
}
