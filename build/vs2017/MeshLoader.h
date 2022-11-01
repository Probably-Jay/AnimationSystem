#pragma once
#include <memory>
#include <string>

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
		Result LoadMeshScene(gef::Scene& scene) override;
		MeshWrapper const * GetMesh(string const& name) const override;
		MeshWrapper const * GetMesh(gef::StringId id) const override;
		std::vector<unsigned int> GetAllMeshIDs() const override;
	private:
		gef::Platform& platform_;
		
		std::map<gef::StringId, unique_ptr<MeshWrapper>> meshes_;
		
	};

	class SingleMeshLoader
	{
	public:
		Result LoadMesh(StringId id, gef::Scene& scene, gef::Platform& platform);
		unique_ptr<MeshWrapper> TakeMesh() {return std::move(mesh_);}
	private:
		unique_ptr<MeshWrapper> mesh_;
	};
}
