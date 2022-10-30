#pragma once
#include <memory>


#include "IMeshLoader.h"
#include "system/platform.h"

using std::unique_ptr;

namespace AnimationSystem
{
	class AnimationSystem3D
	{
	public:

		static unique_ptr<AnimationSystem3D> Create(gef::Platform & gef::Platform & platform_)
		{
			const auto system = new AnimationSystem3D(platform_);
			return unique_ptr<AnimationSystem3D>(system);
		}

		AnimationSystem3D (AnimationSystem3D const&) = delete;
		void operator=(AnimationSystem3D const&) = delete;

		IMeshLoader & MeshLoader() const { return *mesh_loader_; }

	private:
		AnimationSystem3D(gef::Platform & platform_);
		unique_ptr<IMeshLoader> mesh_loader_;

		gef::Platform & gef::Platform & platform_;

	};
}