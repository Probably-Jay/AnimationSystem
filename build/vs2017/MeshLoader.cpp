#include "MeshLoader.h"

using AnimationSystem::IMesh;

AnimationSystem::MeshLoader::MeshLoader(gef::Platform& platform_)
	: platform_(platform_)
{
}

Result AnimationSystem::MeshLoader::LoadMeshScene(gef::Scene& scene)
{

	// todo dont own this
	
	const auto & meshDataList = scene.mesh_data;
	
	if(meshDataList.empty())
		return Result::Error("Scene contained no meshes");

	for (auto & meshData : meshDataList)
	{
		auto mesh = std::unique_ptr<gef::Mesh>(scene.CreateMesh(platform_, meshData));

		if(mesh == nullptr)
			return Result::Error("Mesh " + std::to_string(meshData.name_id) + "could not be created");

		auto wrappedMesh = GefMeshWrapper::Create(std::move(mesh), meshData.name_id);
		meshes_.emplace(meshData.name_id, std::move(wrappedMesh));
	}
	
	return Result::OK();
}

IMesh const * AnimationSystem::MeshLoader::GetMesh(string const & name) const
{
	return GetMesh(gef::GetStringId(name));
}

IMesh const* AnimationSystem::MeshLoader::GetMesh(const gef::StringId id) const
{
	const auto mesh = meshes_.find(id );

	if(mesh == meshes_.end())
		return nullptr;
	
	return mesh->second.get();
}

std::vector<unsigned int> AnimationSystem::MeshLoader::GetAllMeshIDs() const
{
	std::vector<unsigned int> IDs;
	for(auto const& pair: meshes_)
		IDs.push_back(pair.first);
	return IDs;
}
