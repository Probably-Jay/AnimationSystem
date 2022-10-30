#include "AnimationSystem3D.h"

#include "MeshLoader.h"

using namespace AnimationSystem;

AnimationSystem3D::AnimationSystem3D(gef::Platform& platform_)
    : mesh_loader_(new class MeshLoader(platform_))
    , platform_(platform_)
{

}


