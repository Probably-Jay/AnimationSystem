#include "MeshLoader.h"
#include "system/platform.h"

AnimationSystem::PureResult AnimationSystem::MeshLoader::LoadMesh(const gef::StringId id, gef::Scene& scene, gef::Platform & platform)
{
	scene.CreateMeshes(platform);

	const auto & meshList = scene.meshes;
	
	if(meshList.empty())
		return PureResult::Error("Scene contained no meshes");

	if(meshList.size() > 1)
		return PureResult::Error("Loading multiple meshes per scene-file not supported");

	const auto meshFromFile = meshList.front();

	mesh_ = *meshFromFile; 
	
	return PureResult::OK();
}
