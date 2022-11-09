#pragma once
#include <functional>

#include "AnimationClip.h"
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
        [[nodiscard]] virtual ValueResult<std::reference_wrapper<AnimationClip>> GetAnimation(StringId id) const = 0;
        [[nodiscard]] virtual bool HasAnimation(StringId id) const = 0;
        [[nodiscard]] virtual std::optional<string> GetAnimationName(StringId id) const = 0;
    };

}

namespace AnimationSystem
{
    class AnimationContainer final : public IReadOnlyAnimationContainer
    {
    public:
        explicit AnimationContainer(gef::Platform const &platform);


        PureResult LoadAnimations(const string &animationName,
                                  const std::string &filepath,
                                  const std::string &nameWithinFile,
                                  AnimationClip::OptionalConfigOnSetAnimationDelegate configDelegate);

        ValueResult<std::reference_wrapper<AnimationClip>> GetAnimation(StringId id) const override;
        bool HasAnimation(const StringId id) const override {return GefExtensions::HasValue(stringIdTable_, id); }

        std::optional<string> GetAnimationName(StringId id) const override;

    private:
        ValueResult<std::unique_ptr<AnimationClip>> CreateAnimation(const std::string &filepath,
                                                                const std::string &nameWithinFile,
                                                                AnimationClip::OptionalConfigOnSetAnimationDelegate configDelegate,
                                                                const StringId animationId) const;
        std::map<StringId, std::unique_ptr<AnimationClip>> animations_;
        gef::Platform const & platform_;

        mutable gef::StringIdTable stringIdTable_;
    };
}

