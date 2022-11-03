#pragma once
#include <memory>


#include "AnimatorWrapper.h"
#include "IAnimationContainer.h"
#include "IMeshLoader.h"
#include "IStringID.h"
#include "SkeletonLoader.h"
#include "SkinnedMeshContainer.h"
#include "graphics/scene.h"
#include "system/platform.h"
#include "motion_clip_player.h"

#include "AnimationController.h"

#include <AnimationController.h>
#include <functional>

#include "AnimatedObjectContainer.h"
#include "AnimatorConfig.h"


using std::unique_ptr;



namespace AnimationSystem
{
	class AnimatedObjectContainer;

	class AnimationSystem3D
	{
	public:
		static unique_ptr<AnimationSystem3D> Create(gef::Platform & platform_);

		AnimationSystem3D (AnimationSystem3D const&) = delete;
		void operator=(AnimationSystem3D const&) = delete;
		
		CreateEntityResult CreateAnimatedObject(string const & objectNameId, string const & filePath);

		AnimatedObjectContainer const & AnimatedObjects() const {return *animated_objects_container_;}
		
		Result LoadObjectScene(std::string filePath);

		IMeshLoader const & MeshLoader() const { return *mesh_loader_; }
		ISkeletonLoader const & SkeletonLoader() const { return *skeleton_loader_; }

		SkinnedMeshWrapper * GetSkinnedMesh(const StringId id) const {return skinned_mesh_container_->GetSkinnedMesh(id);}


		Result CreateAnimatorForSkinnedMesh(StringId id);

		gef::Scene & GetModelScene() const {return *model_scene_;}
		Result CreateSkinnedMeshFrom(StringId skeletonId) const;

		CreateEntityResult LoadAnimation(const string& nameId, const string& filepath, const string& nameWithinFile) const {return animation_container_->LoadAnimations(nameId, filepath, nameWithinFile);}

		AnimationWrapper* GetAnimation(const StringId id) const {return animation_container_->GetAnimation(id);}


		Result SetAnimatorProperties(const StringId animationId, std::function<void(AnimatorConfig)> const & a) const
		{
			const auto animator = GetAnimator(animationId);
			if(animator == nullptr)
				return Result::Error("Animation could not be found with id" + std::to_string(animationId));

			a(AnimatorConfig(animator->Item()));
			return Result::OK();
		}

		AnimatorWrapper* GetAnimator(const StringId id) const { return GetWrappedValueFromMap(animators_, id); }
		Result CreateAnimatiorForSkinnedMesh(const StringId sMeshId, const StringId animatiorId);
		Result SetAnimation(StringId animatorId, string animName);
	private:
		AnimationSystem3D(gef::Platform & platform_);
		
		unique_ptr<IMeshLoader> mesh_loader_;
		unique_ptr<ISkeletonLoader> skeleton_loader_;

		unique_ptr<SkinnedMeshContainer> skinned_mesh_container_;

		unique_ptr<IAnimationContainer> animation_container_;


		unique_ptr<AnimationController> animationController_;
		
		std::map<StringId, unique_ptr<AnimatorWrapper>> animators_;

		unique_ptr<AnimatedObjectContainer> animated_objects_container_;
		
		gef::Platform & platform_;
		std::unique_ptr<gef::Scene> model_scene_;
	};
}