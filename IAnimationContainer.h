#pragma once
#include <functional>

#include "Animation.h"
#include "AnimatorConfig.h"
#include "PureResult.h"
#include "graphics/scene.h"
#include "system/platform.h"
using std::string;
namespace AnimationSystem
{
    class IReadOnlyAnimationContainer
    {
    public:
        virtual ~IReadOnlyAnimationContainer() = default;
        [[nodiscard]] virtual ValueResult<std::reference_wrapper<Animation>> GetAnimation(StringId id) const = 0;
        [[nodiscard]] virtual bool HasAnimation(StringId id) const = 0;
    };
    class IAnimationContainer : public IReadOnlyAnimationContainer
    {
    public:
        ~IAnimationContainer() override = default;
        virtual PureResult LoadAnimations(const string& nameId, const std::string& filepath, const std::string& nameWithinFile,
                                          const std::function<void(AnimatorConfig)> configDelegate) = 0;
    };
}

namespace AnimationSystem
{
    class AnimationContainer final : public IAnimationContainer
    {
    public:
        explicit AnimationContainer(gef::Platform const & platform);

        PureResult LoadAnimations(const string& animationName, const std::string& filepath, const std::string& nameWithinFile,
                                  const std::function<void(AnimatorConfig)> configDelegate) override;

        ValueResult<std::reference_wrapper<Animation>> GetAnimation(const StringId id) const override;
        bool HasAnimation(const StringId id) const override {return string_id_table_.HasValue(id); }

    private:
        std::map<StringId, std::unique_ptr<Animation>> animations_;
        gef::Platform const & platform_;

        std::unique_ptr<gef::Scene> animationScene;
        mutable gef::StringIdTable string_id_table_;
    };
}

