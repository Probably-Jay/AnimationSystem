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

using std::unique_ptr;



namespace AnimationSystem
{
	class AnimationSystem3D
	{
	public:

		static unique_ptr<AnimationSystem3D> Create(gef::Platform & platform_);

		AnimationSystem3D (AnimationSystem3D const&) = delete;
		void operator=(AnimationSystem3D const&) = delete;

		Result LoadObjectScene(std::string filePath);
		
		IMeshLoader const & MeshLoader() const { return *mesh_loader_; }
		ISkeletonLoader const & SkeletonLoader() const { return *skeleton_loader_; }

		SkinnedMeshWrapper * GetSkinnedMesh(const StringId id) const {return skinned_mesh_container_->GetSkinnedMesh(id);}


		AnimatorWrapper* GetAnimator(const StringId id) const { return GetWrappedValueFromMap(animators_, id); }
		
		Result CreateAnimatorForSkinnedMesh(StringId id);
		
		gef::Scene & GetModelScene() const {return *model_scene_;}
		Result CreateSkinnedMeshFrom(StringId skeletonId) const;
		
		CreateEntityResult LoadAnimation(const string& filepath, const string& name) const {return animation_container_->LoadAnimations(filepath, name);}
		AnimationWrapper* GetAnimation(StringId id) const {return animation_container_->GetAnimation(id);}


	private:
		AnimationSystem3D(gef::Platform & platform_);
		
		unique_ptr<IMeshLoader> mesh_loader_;
		unique_ptr<ISkeletonLoader> skeleton_loader_;

		unique_ptr<SkinnedMeshContainer> skinned_mesh_container_;

		unique_ptr<IAnimationContainer> animation_container_;
		
		std::map<StringId, unique_ptr<AnimatorWrapper>> animators_;
		
		gef::Platform & platform_;
		std::unique_ptr<gef::Scene> model_scene_;
	};
}