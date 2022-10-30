#pragma once
#include <memory>
#include <string>

#include "IMesh.h"
#include "IMeshLoader.h"
#include "Result.h"
#include "graphics/scene.h"


using std::string;
using std::unique_ptr;

namespace AnimationSystem
{
	class MeshLoader final : public IMeshLoader
	{
	public:
		explicit MeshLoader(gef::Platform& platform_);
		Result LoadMeshScene(string const & filepath) override;
		IMesh const * GetMesh(string const& name) const override;
		IMesh const * GetMesh(gef::StringId id) const override;
		std::vector<unsigned int> GetAllMeshIDs() const override;
	private:
		gef::Platform& platform_;
		unique_ptr<gef::Scene> model_scene_;
		
		std::map<gef::StringId, unique_ptr<IMesh>> meshes_;
		
	};
}
