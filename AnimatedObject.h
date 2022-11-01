#pragma once
#include "AnimationController.h"
#include "AnimatorWrapper.h"
#include "SkinnedMeshContainer.h"
#include "SkinnedMeshWrapper.h"
#include "ViewingProtectedWrapper.h"
#include "graphics/scene.h"

namespace AnimationSystem
{
    class AnimatedObject
    {
    public:
        explicit AnimatedObject(const StringId nameId)
            : NameId(nameId)
        {}

        Result Create(gef::Platform& platform,
                      std::unique_ptr<gef::Scene> modelScene);
        
        const StringId NameId;
    private:
        SingleSkinnedMeshContainer skinnedMeshContainer;
        std::unique_ptr<AnimationControllerWrapper> animator;
    };

    typedef OwningProtectedWrapper<AnimatedObject> AnimatedObjectWrapper;
}
