#include "BlendAnimator.h"

void AnimationSystem::BlendAnimator::Init(gef::SkeletonPose const & bindPose)
{
   ActiveAnimator().Init(bindPose);
   AlternateAnimator().Init(bindPose);
}
