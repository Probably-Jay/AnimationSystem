#pragma once
#include "AnimationController.h"
#include "SkinnedMeshContainer.h"
#include "graphics/scene.h"

namespace AnimationSystem
{
    class IAnimatedObject
    {
    public:
        virtual ~IAnimatedObject() = default;
        [[nodiscard]] virtual StringId ID() const = 0;
        [[nodiscard]] virtual IAnimator & Animator() const = 0;
        virtual PureResult UpdateAnimation(float frameTime) = 0;
        virtual void SetTransform(const gef::Matrix44& matrix44) = 0;
        virtual void RenderSelf(gef::Renderer3D& renderer3D) = 0;
    };
    
    class AnimatedObject : public IAnimatedObject
    {
        AnimatedObject(gef::Platform& platform, const StringId nameId);
    public:
        PureResult CreateObjectsFromScene(gef::Platform& platform,
                                          std::unique_ptr<gef::Scene> modelScene);

        [[nodiscard]] StringId ID() const override {return name_id_;}
        [[nodiscard]] IAnimator& Animator() const override {return *animator_;}
        [[nodiscard]] AnimationController& Animator() {return *animator_;}
        
        PureResult UpdateAnimation(float frameTime) override;
        void SetTransform(const gef::Matrix44& transform) override;
        void RenderSelf(gef::Renderer3D& renderer3D) override;

    private:
        const StringId name_id_;
        SkinnedMeshInstance skinned_mesh_container_;
        std::unique_ptr<AnimationController> animator_;

        friend class AnimatedObjectFactory;
    };
}
