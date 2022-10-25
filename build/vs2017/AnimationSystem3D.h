#pragma once
#include <memory>

#include "MeshLoader.h"

using std::unique_ptr;


class AnimationSystem3D
{
public:

	static unique_ptr<AnimationSystem3D> Create()
	{
		const auto instance = new AnimationSystem3D();
		return unique_ptr<AnimationSystem3D>(instance);
	}

	AnimationSystem3D (AnimationSystem3D const&) = delete;
	void operator=(AnimationSystem3D const&) = delete;

	IMeshLoader const & MeshLoader() const { return *mesh_loader_; }

private:
	AnimationSystem3D() = default;

	unique_ptr<IMeshLoader> mesh_loader_;


};

