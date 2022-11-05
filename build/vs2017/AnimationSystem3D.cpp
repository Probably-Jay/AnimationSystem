#include "AnimationSystem3D.h"

#include "AnimatedObject.h"
#include "MeshLoader.h"

#include <optional>

using namespace AnimationSystem;

AnimationSystem3D::AnimationSystem3D(gef::Platform& platform_)
    : mesh_loader_(new class MeshLoader(platform_))
    , skeleton_loader_(new class SkeletonLoader(platform_))
  //  , skinned_mesh_container_(new SkinnedMeshContainer())
    , animation_container_(new AnimationContainer(platform_))
    , animated_objects_factory_(new AnimatedObjectFactory())
    , platform_(platform_)
{

}


unique_ptr<AnimationSystem3D> AnimationSystem3D::Create(gef::Platform& platform_)
{
    const auto system = new AnimationSystem3D(platform_);
    return unique_ptr<AnimationSystem3D>(system);
}


CreateAnimatedObjectResult AnimationSystem3D::CreateAnimatedObject(string const& objectNameId, string const& filePath) const
{
    return animated_objects_factory_->CreateAnimatedObject(objectNameId, filePath, platform_);
}

// PureResult AnimationSystem3D::LoadObjectScene(std::string filePath)
// {
//     // from lab code:
//     
//     // create a new scene object and read in the data from the file
//     // no meshes or materials are created yet
//     // we're not making any assumptions about what the data may be loaded in for
//     model_scene_ = std::make_unique<gef::Scene>();
//     model_scene_->ReadSceneFromFile(platform_, filePath.c_str());
//
//     
//     // we want to render the data stored in the scene file
//     // so create the materials from the material data present in the scene file
//     model_scene_->CreateMaterials(platform_);
//    
//     PureResult result = PureResult::OK();
//     result = mesh_loader_->LoadMeshScene(*model_scene_);
//     if(!result.Successful())
//         return result;
//
//     result = skeleton_loader_->LoadSkeletonScene(*model_scene_);
//     if(!result.Successful())
//         return result;
//     
//     return PureResult::OK();
// }

// PureResult AnimationSystem3D::CreateAnimatorForSkinnedMesh(StringId id)
// {
//     const auto skinnedMesh = skinned_mesh_container_->GetSkinnedMesh(id);
//
//     if(skinnedMesh == nullptr)
//         return PureResult::Error(ERROR_TAG+ "Cannot find skinned mesh with id:" + std::to_string(id));
//     
//     auto animator =  AnimatorWrapper::Create(id);
//
//     animator->Item().Init(skinnedMesh->Item().bind_pose());
//     
//     animators_.emplace(animator->ID(), std::move(animator));
//     return PureResult::OK();
// }
//
// PureResult AnimationSystem3D::CreateSkinnedMeshFrom(const gef::StringId skeletonId) const
// {
//     const auto skeleton = skeleton_loader_->GetSkeleton(skeletonId);
//     if(skeleton == nullptr)
//         return PureResult::Error(ERROR_TAG+ "Skeleton could not be found at "+ std::to_string(skeletonId));
//          
//     const auto mesh = mesh_loader_->GetMesh(skeletonId);
//     if(mesh == nullptr)
//         return PureResult::Error(ERROR_TAG+ "Skeleton could not be found at "+ std::to_string(skeletonId));
//     
//     skinned_mesh_container_->CreateSkinnedMesh(*skeleton, *mesh);
//     return PureResult::OK();
// }
//
// PureResult AnimationSystem3D::SetAnimatorProperties(const StringId animationId,
//     std::function<void(AnimatorConfig)> const& a) const
// {
//     const auto animator = GetAnimator(animationId);
//     if(animator == nullptr)
//         return PureResult::Error("Animation could not be found with id" + std::to_string(animationId));
//
//     a(AnimatorConfig(animator->Item()));
//     return PureResult::OK();
// }
//
// PureResult AnimationSystem3D::CreateAnimationForSkinnedMesh(const StringId sMeshId, const StringId animatiorId) const
// {
//     const auto sMesh = skinned_mesh_container_->GetSkinnedMesh(sMeshId);
//     if(sMesh == nullptr)
//         return PureResult::Error(ERROR_TAG+ "Skinned mesh could not be found at "+ std::to_string(sMeshId));
//
//     const auto animator = GetAnimator(animatiorId);
//     if(animator == nullptr)
//         return PureResult::Error(ERROR_TAG+ "Animator mesh could not be found at "+ std::to_string(animatiorId));
//
//     return PureResult::Error(ERROR_TAG+ "This function is not implimented");
// }
//
// PureResult AnimationSystem3D::SetAnimation(StringId anmiatorId, string animName)
// {
//     const auto animator = GetAnimator(anmiatorId);
//     if(animator == nullptr)
//         return PureResult::Error(ERROR_TAG+ "Animator mesh could not be found at "+ std::to_string(anmiatorId));
//
//     const auto animation= animation_container_->GetAnimation(animName);
//     if(animation == nullptr)
//         return PureResult::Error(ERROR_TAG+ "Animation could not be found with id" + animName);
//
//   //  std::optional<bool> a;
//     
//     animator->Item().set_clip(&animation->Item());
//     return PureResult::OK();
// }

PureResult AnimationSystem3D::CreateAnimationFor(IAnimatedObject const& readonlyAnimObject, string const& animationName,
                                                 string const& fileName, string const& nameWithinFile,
                                                 std::function<void(AnimatorConfig&)> const& configurationDelegate)
{
    auto animatedObjectResult = animated_objects_factory_->GetObject(readonlyAnimObject);
    if(!animatedObjectResult.IsError())
        return animatedObjectResult.ToPureResult();

    auto & animatedObject = animatedObjectResult.Take().get();
    PureResult createAnimationResult = animatedObject.Animator().CreateAnimation(animationName, fileName, nameWithinFile, configurationDelegate);
    return createAnimationResult;
}



