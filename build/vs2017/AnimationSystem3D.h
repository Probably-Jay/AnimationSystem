#pragma once
#include <memory>


#include "graphics/scene.h"

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

		[[nodiscard]] CreateAnimatedObjectResult CreateAnimatedObject(string const & objectNameId, string const & filePath) const;

		PureResult CreateAnimationFor(IAnimatedObject const& readonlyAnimObject, string const& animationName,
		                              string const& fileName, string const& nameWithinFile, std::function<void(AnimatorConfig)> const configurationDelegate);
	private:
		AnimationSystem3D(gef::Platform & platform_);
		
		unique_ptr<AnimatedObjectFactory> animated_objects_factory_;
		
		gef::Platform & platform_;
	};
}