#include "MeshLoader.h"

using AnimationSystem::IMesh;

AnimationSystem::MeshLoader::MeshLoader(gef::Platform& platform_): platform_(platform_)
{
	
}

Result AnimationSystem::MeshLoader::LoadMeshScene(const std::string& filepath)
{
	// from lab code:
	
	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	model_scene_ = std::make_unique<gef::Scene>();
	model_scene_->ReadSceneFromFile(platform_, filepath.c_str());

	// we do want to render the data stored in the scene file
	// so lets create the materials from the material data present in the scene file
	model_scene_->CreateMaterials(platform_);

	const auto & meshDataList = model_scene_->mesh_data;
	
	if(meshDataList.empty())
		return Result::Error("Scene at " + filepath +" contained no meshes");

	for (auto & meshData : meshDataList)
	{
		auto mesh = std::unique_ptr<gef::Mesh>(model_scene_->CreateMesh(platform_, meshData));
		if(mesh == nullptr)
			return Result::Error("Mesh " + std::to_string(meshData.name_id) + "could not be created");

		auto wrappedMesh = GefMeshWrapper::Create(std::move(mesh));
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
