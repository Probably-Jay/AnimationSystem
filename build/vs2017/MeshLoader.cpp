#include "MeshLoader.h"

using AnimationSystem::MeshWrapper;

AnimationSystem::MeshLoader::MeshLoader(gef::Platform& platform_)
	: platform_(platform_)
{
}

AnimationSystem::Result AnimationSystem::MeshLoader::LoadMeshScene(gef::Scene& scene)
{
	scene.CreateMeshes(platform_);

	const auto & meshList = scene.meshes;
	const auto & meshDataList = scene.mesh_data;
	
	if(meshList.empty())
		return Result::Error("Scene contained no meshes");

	// todo this properly
	
	auto meshDataIter = meshDataList.begin();
	for(auto meshIter = meshList.begin(); meshIter != meshList.end(); ++meshIter, ++meshDataIter)
	{
		gef::StringId uniqueID = meshDataIter->name_id;
		
		auto wrappedMesh = MeshWrapper::Create(**meshIter, meshDataIter->name_id);

		meshes_.emplace(uniqueID, std::move(wrappedMesh));
	}
	
	return Result::OK();

	// {
	// 	auto mesh = std::unique_ptr<gef::Mesh>(scene.CreateMesh(platform_, meshData));
	//
	// 	if(mesh == nullptr)
	// 		return Result::Error("Mesh " + std::to_string(meshData.name_id) + "could not be created");
	//
	// 	auto wrappedMesh = IMesh::Create(std::move(mesh), meshData.name_id);
	// 	meshes_.emplace(meshData.name_id, std::move(wrappedMesh));
	// }
}

MeshWrapper const * AnimationSystem::MeshLoader::GetMesh(string const & name) const
{
	return GetMesh(gef::GetStringId(name));
}

MeshWrapper const* AnimationSystem::MeshLoader::GetMesh(const gef::StringId id) const
{
	const auto mesh = meshes_.find(id);

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
