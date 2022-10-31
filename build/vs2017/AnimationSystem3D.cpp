#include "AnimationSystem3D.h"

#include "MeshLoader.h"

using namespace AnimationSystem;

Result AnimationSystem3D::LoadObjectScene(std::string filePath)
{
    // from lab code:
    
    // create a new scene object and read in the data from the file
    // no meshes or materials are created yet
    // we're not making any assumptions about what the data may be loaded in for
    model_scene_ = std::make_unique<gef::Scene>();
    model_scene_->ReadSceneFromFile(platform_, filePath.c_str());

    
    // we want to render the data stored in the scene file
    // so create the materials from the material data present in the scene file
    model_scene_->CreateMaterials(platform_);
   
    Result result = Result::OK();
    result = mesh_loader_->LoadMeshScene(*model_scene_);
    if(!result.Successful())
        return result;

    result = skeleton_loader_->LoadSkeletonScene(*model_scene_);
    if(!result.Successful())
        return result;
    
    return Result::OK();
}

Result AnimationSystem3D::CreateSkinnedMeshFrom(const gef::StringId skeletonId) const
{
    const auto skeleton = skeleton_loader_->GetSkeleton(skeletonId);
    if(skeleton == nullptr)
        return Result::Error("Skeleton could not be found at "+ std::to_string(skeletonId));
         
    const auto mesh = mesh_loader_->GetMesh(skeletonId);
    if(mesh == nullptr)
        return Result::Error("Skeleton could not be found at "+ std::to_string(skeletonId));
    
    skinned_mesh_container_->CreateSkinnedMesh(*skeleton, *mesh);
    return Result::OK();
}

AnimationSystem3D::AnimationSystem3D(gef::Platform& platform_)
    : mesh_loader_(new class MeshLoader(platform_))
    , skeleton_loader_(new class SkeletonLoader(platform_))
    , skinned_mesh_container_(new SkinnedMeshContainer())
    , platform_(platform_)
{

}


