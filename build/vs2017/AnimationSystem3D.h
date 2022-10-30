#pragma once
#include <memory>


#include "IMeshLoader.h"
#include "SkeletonLoader.h"
#include "SkinnedMeshContainer.h"
#include "graphics/scene.h"
#include "system/platform.h"

using std::unique_ptr;

namespace AnimationSystem
{
	class AnimationSystem3D
	{
	public:

		static unique_ptr<AnimationSystem3D> Create(gef::Platform & platform_)
		{
			const auto system = new AnimationSystem3D(platform_);
			return unique_ptr<AnimationSystem3D>(system);
		}

		AnimationSystem3D (AnimationSystem3D const&) = delete;
		void operator=(AnimationSystem3D const&) = delete;

		IMeshLoader const & MeshLoader() const { return *mesh_loader_; }
		ISkeletonLoader const & SkeletonLoader() const { return *skeleton_loader_; }
		Result LoadObjectScene(std::string filePath);

		gef::Scene & GetModelScene() const {return *model_scene_;}
		
	private:
		AnimationSystem3D(gef::Platform & platform_);
		
		unique_ptr<IMeshLoader> mesh_loader_;
		unique_ptr<ISkeletonLoader> skeleton_loader_;

		unique_ptr<SkinnedMeshContainer> skinnedMeshes;

		
		gef::Platform & platform_;
		std::unique_ptr<gef::Scene> model_scene_;
	};
}