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

#include "AnimatedObjectFactory.h"
#include "AnimatorConfig.h"


using std::unique_ptr;



namespace AnimationSystem
{
	class AnimatedObjectFactory;

	class AnimationSystem3D
	{
	public:
		static unique_ptr<AnimationSystem3D> Create(gef::Platform & platform_);

		AnimationSystem3D (AnimationSystem3D const&) = delete;
		void operator=(AnimationSystem3D const&) = delete;
		
		CreateAnimatedObjectResult CreateAnimatedObject(string const & objectNameId, string const & filePath) const;

		AnimatedObjectFactory const & AnimatedObjects() const {return *animated_objects_factory_;}
		
		//PureResult LoadObjectScene(std::string filePath);

		IMeshLoader const & MeshLoader() const { return *mesh_loader_; }
		ISkeletonLoader const & SkeletonLoader() const { return *skeleton_loader_; }

//		SkinnedMeshWrapper * GetSkinnedMesh(const StringId id) const {return skinned_mesh_container_->GetSkinnedMesh(id);}


		//PureResult CreateAnimatorForSkinnedMesh(StringId id);

		gef::Scene & GetModelScene() const {return *model_scene_;}
		//PureResult CreateSkinnedMeshFrom(StringId skeletonId) const;

		//ValueResult LoadAnimation(const string& nameId, const string& filepath, const string& nameWithinFile) const {return animation_container_->LoadAnimations(nameId, filepath, nameWithinFile);}

		//AnimationWrapper* GetAnimation(const StringId id) const {return animation_container_->GetAnimation(id);}


	//	PureResult SetAnimatorProperties(const StringId animationId, std::function<void(AnimatorConfig)> const & a) const;

	//	AnimatorWrapper* GetAnimator(const StringId id) const { return GetWrappedValueFromMap(animators_, id); }
		//PureResult CreateAnimationForSkinnedMesh(const StringId sMeshId, const StringId animatiorId) const;
	//	PureResult SetAnimation(StringId animatorId, string animName);

		PureResult CreateAnimationFor(IAnimatedObject const& readonlyAnimObject, string const& animationName,
		                              string const& fileName, string const& nameWithinFile, std::function<void(AnimatorConfig)> const configurationDelegate);
	private:
		AnimationSystem3D(gef::Platform & platform_);
		
		unique_ptr<IMeshLoader> mesh_loader_;
		unique_ptr<ISkeletonLoader> skeleton_loader_;

		//unique_ptr<SkinnedMeshContainer> skinned_mesh_container_;

		unique_ptr<IAnimationContainer> animation_container_;


		unique_ptr<AnimationController> animationController_;
		
		std::map<StringId, unique_ptr<AnimatorWrapper>> animators_;

		unique_ptr<AnimatedObjectFactory> animated_objects_factory_;
		
		gef::Platform & platform_;
		std::unique_ptr<gef::Scene> model_scene_;
	};
}