#include "animated_mesh_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>

#include "IMesh.h"

AnimatedMeshApp::AnimatedMeshApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(nullptr),
	renderer_3d_(nullptr),
	input_manager_(nullptr),
	font_(nullptr),
	//mesh_(nullptr),
	player_(nullptr),
	//model_scene_(nullptr),
	walk_anim_(nullptr),
	animation_system_(nullptr)
{
}

void AnimatedMeshApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	animation_system_ = AnimationSystem::AnimationSystem3D::Create(platform_);

	InitFont();
	SetupCamera();
	SetupLights();

	LoadMeshAndAnimation();
}

void AnimatedMeshApp::LoadMeshAndAnimation()
{


	animation_system_->LoadObjectScene("tesla/tesla.scn");

	const auto & meshLoader = animation_system_->MeshLoader();

	const auto ids = meshLoader.GetAllMeshIDs();

	const auto wrappedMesh = meshLoader.GetMesh(ids.front());
	
	
	
	
	// // create a new scene object and read in the data from the file
	// // no meshes or materials are created yet
	// // we're not making any assumptions about what the data may be loaded in for
	// model_scene_ = new gef::Scene();
	// model_scene_->ReadSceneFromFile(platform_, "tesla/tesla.scn");
	//
	// // we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	// model_scene_->CreateMaterials(platform_);
	//
	// // if there is mesh data in the scene, create a mesh to draw from the first mesh
	// mesh_ = GetFirstMesh(model_scene_);
	
	// get the first skeleton in the scene
	gef::Skeleton* skeleton = GetFirstSkeleton(&animation_system_->GetModelScene());

	if (skeleton)
	{
		player_ = new gef::SkinnedMeshInstance(*skeleton);
		anim_player_.Init(player_->bind_pose());
		player_->set_mesh(&wrappedMesh->Mesh());
	}


	// anims
	walk_anim_ = LoadAnimation("tesla/tesla@walk.scn", "");

	if (walk_anim_)
	{
		anim_player_.set_clip(walk_anim_);
		anim_player_.set_looping(true);
		anim_player_.set_anim_time(0.0f);
	}
}


void AnimatedMeshApp::CleanUp()
{
	CleanUpFont();

	delete player_;
	player_ = nullptr;

	delete walk_anim_;
	walk_anim_ = nullptr;

	// delete mesh_;
	// mesh_ = nullptr;

	// delete model_scene_;
	// model_scene_ = nullptr;

	delete input_manager_;
	input_manager_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	delete renderer_3d_;
	renderer_3d_ = nullptr;
}

bool AnimatedMeshApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;


	// read input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// controller input
		gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
		if (controller_manager)
		{
		}

		// keyboard input
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
		}
	}

	if(player_)
	{
		// update the pose in the anim player from the animation
		anim_player_.Update(frame_time, player_->bind_pose());

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		player_->UpdateBoneMatrices(anim_player_.pose());
	}

	// build a transformation matrix that will position the character
	// use this to move the player around, scale it, etc.
	if (player_)
	{
		gef::Matrix44 player_transform;
		player_transform.SetIdentity();
		player_->set_transform(player_transform);
	}

	return true;
}

void AnimatedMeshApp::Render()
{
	// setup view and projection matrices
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_, camera_lookat_, camera_up_);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	// draw meshes here
	renderer_3d_->Begin();

	// draw the player, the pose is defined by the bone matrices
	if(player_)
		renderer_3d_->DrawSkinnedMesh(*player_, player_->bone_matrices());

	renderer_3d_->End();

	// setup the sprite renderer, but don't clear the frame buffer
	// draw 2D sprites here
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}
void AnimatedMeshApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void AnimatedMeshApp::CleanUpFont()
{
	delete font_;
	font_ = nullptr;
}

void AnimatedMeshApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void AnimatedMeshApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void AnimatedMeshApp::SetupCamera()
{
	// initialise the camera settings
	camera_eye_ = gef::Vector4(-1.0f, 1.0f, 4.0f);
	camera_lookat_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 1000.f;
}


gef::Skeleton* AnimatedMeshApp::GetFirstSkeleton(const gef::Scene* scene) const
{
	// check to see if there is a skeleton in the the scene file
	if (scene == nullptr || scene->skeletons.empty())
		return nullptr;

	// if so, pull out the bind pose and create an array of matrices
	// that wil be used to store the bone transformations
	return scene->skeletons.front();
}

gef::Mesh* AnimatedMeshApp::GetFirstMesh(const gef::Scene* scene) const
{
	if (scene == nullptr || scene->mesh_data.empty())
		return nullptr;

	return animation_system_->GetModelScene().CreateMesh(platform_, scene->mesh_data.front());
}

gef::Animation* AnimatedMeshApp::LoadAnimation(const char* anim_scene_filename, const char* anim_name) const
{
	auto anim_scene = gef::Scene{};
	
	if (!anim_scene.ReadSceneFromFile(platform_, anim_scene_filename))
		return nullptr;

	const auto anim_node_iter =
		anim_name != nullptr
			? anim_scene.animations.find(gef::GetStringId(anim_name))
			: anim_scene.animations.begin();

	if (anim_node_iter == anim_scene.animations.end())
		return nullptr;

	return new gef::Animation(*anim_node_iter->second);
}
