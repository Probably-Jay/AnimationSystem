#include "BlendAnimator.h"

AnimationSystem::BlendAnimator::BlendAnimator(): current_animator_(Controller::Left) {
}

AnimationSystem::AnimatorWrapper& AnimationSystem::BlendAnimator::ActiveAnimator() const {return controllers_.Get(current_animator_);}

gef::SkeletonPose AnimationSystem::BlendAnimator::UpdateAnimationSimple(float const frameTime,
   gef::SkeletonPose const& skeletonPose) {
   return ActiveAnimator().UpdateAnimation(frameTime, skeletonPose);
}

void AnimationSystem::BlendAnimator::UpdateTransition(float frameTime) {
   auto transition = transition_.value();
   transition.AccumulateTime(frameTime);
   if(transition.InTransition())
      return;

   transition_.reset();
}

gef::SkeletonPose AnimationSystem::BlendAnimator::UpdateAnimationBlended(float frameTime,
   gef::SkeletonPose const& skeletonPose) {
   UpdateTransition(frameTime);

   if(!InTransition()) // transition just ended this update
      return UpdateAnimationSimple(frameTime, skeletonPose);

   auto const activePose = ActiveAnimator().UpdateAnimation(frameTime, skeletonPose);
   auto const previousPose = AlternateAnimator().UpdateAnimation(frameTime, skeletonPose);

   auto const lerpValue = transition_.value().LerpValue();
            
   auto blendedPose = skeletonPose;
   blendedPose.Linear2PoseBlend(previousPose, activePose, lerpValue);
            
   return blendedPose;
}

gef::SkeletonPose AnimationSystem::BlendAnimator::UpdateAnimation(float const frameTime,
   gef::SkeletonPose const& skeletonPose) {
   if(!InTransition())
      return UpdateAnimationSimple(frameTime, skeletonPose);

   return UpdateAnimationBlended(frameTime, skeletonPose);
}

AnimationSystem::AnimatorWrapper& AnimationSystem::BlendAnimator::AlternateAnimator() const {return controllers_.Get(ControllerOther(current_animator_));}

bool AnimationSystem::BlendAnimator::InTransition() const {return transition_.has_value();}

AnimationSystem::AnimatorWrapper& AnimationSystem::BlendAnimator::AnimationControllerPair::Get(
   Controller const controller) const {return map_.at(controller)();}

AnimationSystem::BlendAnimator::Controller AnimationSystem::BlendAnimator::ControllerOther(Controller const current) {
   switch (current)
   {
   case Controller::Left: return Controller::Right;
   case Controller::Right: return Controller::Left;
   }
   throw;
}

void AnimationSystem::BlendAnimator::Init(gef::SkeletonPose const & bindPose)
{
   ActiveAnimator().Init(bindPose);
   AlternateAnimator().Init(bindPose);
}

