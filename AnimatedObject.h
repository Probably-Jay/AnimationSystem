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
        virtual IAnimator & Animator() const = 0;
        virtual void UpdateAnimation(float frameTime) = 0;
        virtual void set_transform(const gef::Matrix44& matrix44) = 0;
        virtual void RenderSelf(gef::Renderer3D& renderer3D) = 0;
    };
    
    class AnimatedObject : public IAnimatedObject
    {
    public:
        explicit AnimatedObject(gef::Platform& platform, const StringId nameId)
            : name_id_(nameId)
            , animator(std::make_unique<AnimationController>(nameId, platform))
        {}
        
        PureResult CreateObjectsFromScene(gef::Platform& platform,
                      std::unique_ptr<gef::Scene> modelScene);

        StringId ID() const override {return name_id_;}

      
        IAnimator& Animator() const override {return *animator;}
        AnimationController& Animator() {return *animator;}
        void UpdateAnimation(float frameTime) override;
        void set_transform(const gef::Matrix44& transform) override;
        void RenderSelf(gef::Renderer3D& renderer3D) override;

    private:
        const StringId name_id_;
        SingleSkinnedMeshContainer skinnedMeshContainer;
        std::unique_ptr<AnimationController> animator;
    };

    typedef OwningProtectedWrapper<AnimatedObject> AnimatedObjectWrapper;
  
}
