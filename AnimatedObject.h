#pragma once
#include "AnimationController.h"
#include "AnimatorWrapper.h"
#include "SkinnedMeshContainer.h"
#include "SkinnedMeshWrapper.h"
#include "ViewingProtectedWrapper.h"
#include "graphics/scene.h"

namespace AnimationSystem
{
    class IAnimatedObject
    {
    public:
        virtual ~IAnimatedObject() = default;
        virtual StringId ID() const = 0;
    };
    
    class AnimatedObject : public IAnimatedObject
    {
    public:
        explicit AnimatedObject(gef::Platform& platform, const StringId nameId)
            : name_id_(nameId)
            , animator(AnimationControllerWrapper::Create(nameId, nameId, platform))
        {}
        
        Result CreateObjectsFromScene(gef::Platform& platform,
                      std::unique_ptr<gef::Scene> modelScene);

        StringId ID() const override {return name_id_;}
    private:
        const StringId name_id_;
        SingleSkinnedMeshContainer skinnedMeshContainer;
        std::unique_ptr<AnimationControllerWrapper> animator;
    };

    typedef OwningProtectedWrapper<AnimatedObject> AnimatedObjectWrapper;
  
}
