﻿#pragma once
#include <functional>

#include "AnimatorConfig.h"
#include "ViewingProtectedWrapper.h"
#include "animation/animation.h"

using gef::StringId;

namespace AnimationSystem
{
  class Animation
  {
  public:
    
    Animation(gef::Animation& animation, const StringId id,
    const std::function<void(AnimatorConfig)> configDelegate)
      : animation_(animation)
      , id_(id)
      , config_delegate_(configDelegate)
    {
    }
    Animation(gef::Animation& animation, const StringId id)
      : animation_(animation)
      , id_(id)
      , config_delegate_([](auto _){})
    {
    }

    [[nodiscard]] gef::Animation const & GetAnimation() const {return animation_;}
    [[nodiscard]] StringId ID() const {return id_;}
    void ApplyConfig(AnimatorConfig animator) const
    {
      config_delegate_(animator);
    }
    
  private:
    gef::Animation& animation_;
    StringId id_;
    std::function<void(AnimatorConfig)> const config_delegate_;
  };

 
}
