#pragma once
#include <memory>
#include <string>

#include "IMesh.h"
#include "IMeshLoader.h"
#include "Result.h"
#include "graphics/scene.h"


using std::string;

namespace AnimationSystem
{
	class MeshLoader final : public IMeshLoader
	{
	public:
		explicit MeshLoader(gef::Platform& platform_);
		Result LoadMeshScene(string const & filepath) override;
		IMesh const * GetMesh(string const& name) override;
	private:
		gef::Platform& platform_;
		gef::Scene model_scene_;
		
		std::map<string, std::unique_ptr<IMesh>> meshes;
		
	};
}
