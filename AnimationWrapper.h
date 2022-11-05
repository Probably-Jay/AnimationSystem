#pragma once
#include "AnimatorConfig.h"
#include "ViewingProtectedWrapper.h"
#include "animation/animation.h"

namespace AnimationSystem
{
  // typedef OwningProtectedWrapper<gef::Animation> AnimationWrapper;
  class Animation
  {
  public:
    
    Animation(std::unique_ptr<gef::Animation> animation, const StringId id,
    const std::function<void(AnimatorConfig)>& configDelegate)
      : animation_(std::move(animation))
      , id_(id)
      , config_delegate_(configDelegate)
    {
    }
    Animation(std::unique_ptr<gef::Animation> animation, const StringId id)
      : animation_(std::move(animation))
      , id_(id)
      , config_delegate_([](auto _){})
    {
    }

    gef::Animation const & GetAnimation() const {return *animation_;}
    StringId ID() const {return id_;}
    void ApplyConfig(AnimatorConfig animator) const {config_delegate_(animator);}
    
  private:
    std::unique_ptr<gef::Animation> animation_{};
    StringId id_;
    std::function<void(AnimatorConfig)> const & config_delegate_;
  };


 
}
