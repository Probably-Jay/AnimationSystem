#pragma once
#include "AnimationWrapper.h"
#include "Result.h"
#include "system/platform.h"
using std::string;
namespace AnimationSystem
{
    class IAnimationContainer
    {
    public:
        virtual ~IAnimationContainer() = default;
        virtual CreateEntityResult LoadAnimations(const string& filepath, string animationName) = 0;
        virtual AnimationWrapper * GetAnimation(StringId id) const = 0;
        
    };
}namespace AnimationSystem
{
    class AnimationContainer : public IAnimationContainer
    {
    public:
        explicit AnimationContainer(gef::Platform const & platform);

        CreateEntityResult LoadAnimations(const string& filepath, string animationName) override;
        
        AnimationWrapper * GetAnimation(const StringId id) const override {return GetWrappedValueFromMap(animations_, id);}
        
    private:

        std::map<StringId, std::unique_ptr<AnimationWrapper>> animations_;
        gef::Platform const & platform_;
    };
}

