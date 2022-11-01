#pragma once
#include "AnimationWrapper.h"
#include "Result.h"
#include "system/platform.h"
using std::string;
namespace AnimationSystem
{
    class IReadOnlyAnimationContainer
    {
    public:
        virtual ~IReadOnlyAnimationContainer() = default;
        virtual AnimationWrapper * GetAnimation(const string id) const =0;
        virtual AnimationWrapper * GetAnimation(StringId id) const = 0;
        virtual bool HasAnimation(StringId id) const = 0;
    };
    class IAnimationContainer : public IReadOnlyAnimationContainer
    {
    public:
        ~IAnimationContainer() override = default;
        virtual CreateEntityResult LoadAnimations(const string& nameId, const std::string& filepath, const std::string& nameWithinFile) =0;
    };
}

namespace AnimationSystem
{
    class AnimationContainer : public IAnimationContainer
    {
    public:
        explicit AnimationContainer(gef::Platform const & platform);

        CreateEntityResult LoadAnimations(const string& nameId, const std::string& filepath, const std::string& nameWithinFile)override;

        AnimationWrapper * GetAnimation(const string id) const override {return  GetWrappedValueFromMap(animations_, gef::GetStringId(id));}
        AnimationWrapper * GetAnimation(const StringId id) const override {return GetWrappedValueFromMap(animations_, id);}
        bool HasAnimation(const StringId id) const override {return GetAnimation(id) != nullptr; }

    private:
        std::map<StringId, std::unique_ptr<AnimationWrapper>> animations_;
        gef::Platform const & platform_;

        mutable gef::StringIdTable string_id_table_;
    };
}

