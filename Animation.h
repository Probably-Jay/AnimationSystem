#pragma once
#include <functional>
#include <optional>

#include "AnimatorConfig.h"
#include "ViewingProtectedWrapper.h"
#include "animation/animation.h"
#include "graphics/scene.h"

using gef::StringId;

namespace AnimationSystem
{
  class Animation
  {
  public:
    
    Animation(std::unique_ptr<gef::Scene> animationScene, gef::Animation& animation, const StringId id,
    std::optional<std::function<void(IAnimatorConfig&)> const> const configDelegate)
      : animation_(animation)
      , id_(id)
      , config_delegate_(configDelegate)
      , animationScene_(std::move(animationScene))
    {
    }

    [[nodiscard]] gef::Animation const & GetAnimation() const {return animation_;}
    [[nodiscard]] StringId ID() const {return id_;}
    void ApplyConfig(IAnimatorConfig& animator) const
    {
      if(config_delegate_.has_value())
        config_delegate_.value()(animator);
    }
    
  private:
    gef::Animation& animation_;
    StringId id_;
    std::optional<std::function<void(IAnimatorConfig&)>> const config_delegate_;
    std::unique_ptr<gef::Scene> const animationScene_;
  };

 
}
