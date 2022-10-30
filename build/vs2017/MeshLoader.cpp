#include "MeshLoader.h"

AnimationSystem::MeshLoader::MeshLoader(gef::Platform& platform_)
{
}

Result AnimationSystem::MeshLoader::LoadMeshScene(const std::string& filepath)
{
	//model_scene_->ReadSceneFromFile(platform_, "tesla/tesla.scn");



	
	return Result::OK();
}

IMesh const * AnimationSystem::MeshLoader::GetMesh(string name)
{
	// todo get Mesh
	return null_pointer_;
}
