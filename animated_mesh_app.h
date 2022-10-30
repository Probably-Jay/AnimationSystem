#ifndef _ANIMATED_MESH_APP_H
#define _ANIMATED_MESH_APP_H

#include <animation/animation.h>
#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include <graphics/skinned_mesh_instance.h>

#include "AnimationSystem3D.h"
#include "motion_clip_player.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class Scene;
	class Skeleton;
	class InputManager;
}

class AnimatedMeshApp : public gef::Application
{
public:
	AnimatedMeshApp(gef::Platform& platform);
	void LoadMeshAndAnimation();
	void Init();

	gef::Skeleton* GetFirstSkeleton(const gef::Scene* scene) const;

	gef::Mesh* GetFirstMesh(const gef::Scene* scene) const;

	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SetupCamera();
	gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name) const;


	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::Font* font_;

	float fps_;

	//gef::Mesh * mesh_;
	gef::SkinnedMeshInstance* player_;

	//gef::Scene* model_scene_;

	gef::Vector4 camera_eye_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	MotionClipPlayer anim_player_;
	gef::Animation* walk_anim_;

	std::unique_ptr<AnimationSystem::AnimationSystem3D> animation_system_;
};

#endif // _ANIMATED_MESH_APP_H
