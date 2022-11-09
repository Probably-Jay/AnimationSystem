#pragma once
#include <functional>
#include <optional>

#include "AnimatorConfig.h"
#include "animation/animation.h"
#include "graphics/scene.h"

using gef::StringId;

namespace AnimationSystem
{
  class Animation
  {
  public:
    using OptionalConfigOnSetAnimationDelegate = std::optional<std::function<void(IAnimatorConfig& animatorConfig)> const>;
    using OptionalConfigOnTransitionAnimationDelegate = std::optional<std::function<void(IAnimatorConfig& animatorConfig, float transitionPercent)> const>;

    Animation(std::unique_ptr<gef::Scene> animationScene, gef::Animation& animation, const StringId id,
              OptionalConfigOnSetAnimationDelegate configDelegate)
      : animation_(animation)
      , id_(id)
      , config_delegate_(std::move(configDelegate))
      , animation_scene_(std::move(animationScene))
    {
    }

    [[nodiscard]] gef::Animation const & GetAnimation() const {return animation_;}
    [[nodiscard]] StringId ID() const {return id_;}
    
    void ApplyConfig(IAnimatorConfig& animator) const noexcept(false)
    {
      if(config_delegate_.has_value())
        config_delegate_.value()(animator);
    }
    
  private:
    gef::Animation& animation_;
    StringId id_;
    OptionalConfigOnSetAnimationDelegate const config_delegate_;
    std::unique_ptr<gef::Scene> const animation_scene_;
  };

 
}
