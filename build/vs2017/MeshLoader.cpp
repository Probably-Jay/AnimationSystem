#include "MeshLoader.h"

AnimationSystem::MeshLoader::MeshLoader(gef::Platform& platform_): platform_(platform_)
{
	
}

Result AnimationSystem::MeshLoader::LoadMeshScene(const std::string& filepath)
{
	//model_scene_->ReadSceneFromFile(platform_, "tesla/tesla.scn");



	
	return Result::OK();
}

IMesh const * AnimationSystem::MeshLoader::GetMesh(string const & name)
{
	const auto mesh = meshes.find(name);
	
	if(mesh == meshes.end())
		return nullptr;
	
	return mesh->second.get();
}
