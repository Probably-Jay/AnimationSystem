#pragma once
#include <functional>

#include "Animation.h"
#include "AnimatorConfig.h"
#include "GefExtentions.h"
#include "PureResult.h"
#include "graphics/scene.h"
#include "system/platform.h"
using std::string;
using gef::StringId;
namespace AnimationSystem
{
    class IReadOnlyAnimationContainer
    {
    public:
        virtual ~IReadOnlyAnimationContainer() = default;
        [[nodiscard]] virtual ValueResult<std::reference_wrapper<Animation>> GetAnimation(StringId id) const = 0;
        [[nodiscard]] virtual bool HasAnimation(StringId id) const = 0;
        [[nodiscard]] virtual std::optional<string> GetAnimationName(StringId id) const = 0;
    };
    class IAnimationContainer : public IReadOnlyAnimationContainer
    {
    public:
        ~IAnimationContainer() override = default;
        virtual PureResult LoadAnimations(const string& nameId, const std::string& filepath, const std::string& nameWithinFile,
                                          std::optional<std::function<void(IAnimatorConfig&)> const> configDelegate) = 0;
    };
}

namespace AnimationSystem
{
    class AnimationContainer final : public IAnimationContainer
    {
    public:
        explicit AnimationContainer(gef::Platform const & platform);

        PureResult LoadAnimations(const string& animationName, const std::string& filepath, const std::string& nameWithinFile,
                                   std::optional<std::function<void(IAnimatorConfig&)> const>  configDelegate) override;

        ValueResult<std::reference_wrapper<Animation>> GetAnimation(StringId id) const override;
        bool HasAnimation(const StringId id) const override {return GefExtensions::HasValue(stringIdTable_, id); }

        std::optional<string> GetAnimationName(StringId id) const override;

    private:
        std::map<StringId, std::unique_ptr<Animation>> animations_;
        gef::Platform const & platform_;

        mutable gef::StringIdTable stringIdTable_;
    };
}

