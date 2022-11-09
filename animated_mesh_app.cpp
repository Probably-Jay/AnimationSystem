#include "animated_mesh_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <animation/skeleton.h>

#include "AnimatedObject.h"


AnimatedMeshApp::AnimatedMeshApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(nullptr),
	renderer_3d_(nullptr),
	input_manager_(nullptr),
	font_(nullptr),
	player_(nullptr),
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

AnimationSystem::PureResult AnimatedMeshApp::LoadMeshAndAnimation()
{
	auto createPlayerResult = animation_system_->CreateAnimatedObject("Player", "xbot/xbot.scn") ;
	
	if(createPlayerResult.IsError())
		return createPlayerResult.ToPureResult();

	player_ = createPlayerResult.Take();

	if(auto animationResult =
		animation_system_->CreateAnimationFor(*player_,
		                                      "Walk","xbot/xbot@walking_inplace.scn","",
		                                      [](IAnimatorConfig& animPlayer)
		                                      {
		                                      	animPlayer.SetAnimationTime(0);

			                                      animPlayer.SetLooping(true);
		                                      });
		    animationResult.IsError())
	{
		return animationResult;
	}
	
	if(auto animationResult =
			animation_system_->CreateAnimationFor(*player_,
			                                      "Run","xbot/xbot@running_inplace.scn","",
			                                      [](IAnimatorConfig& animPlayer)
			                                      {
			                                      	animPlayer.SetAnimationTime(0);

				                                      animPlayer.SetLooping(true);
			                                      });
		animationResult.IsError())
	{
		return animationResult;
	}


	if(auto result = player_->Animator().SetAnimation("Walk"); result.IsError())
		return result;

	
	return AnimationSystem::PureResult::OK();
}


void AnimatedMeshApp::CleanUp()
{
	CleanUpFont();
	
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
			if(keyboard->IsKeyPressed(gef::Keyboard::KC_W))
			{
				if(player_->Animator().CurrentAnimationName() == "Walk")
					player_->Animator().SetAnimation("Run", 3,
						[](IAnimatorConfig& animator, IAnimatorConfig& previousAnimator, float transitionTime)
					{
						auto const currentToPreviousScaleFactor = 1.f/animator.GetDurationScalingFactor(previousAnimator);
						auto const previousToCurrentScaleFactor = 1.f/previousAnimator.GetDurationScalingFactor(animator);
						
						auto const currentAnimatorSpeedVal = std::lerp(currentToPreviousScaleFactor, 1.f, transitionTime);
						animator.SetPlaybackSpeed(currentAnimatorSpeedVal);

						auto const previousAnimatorSpeedVal = std::lerp(1.f, previousToCurrentScaleFactor, transitionTime);
						previousAnimator.SetPlaybackSpeed(previousAnimatorSpeedVal);
					});
				else
					player_->Animator().SetAnimation("Walk", 2,
						[](IAnimatorConfig& animator, IAnimatorConfig& previousAnimator, float transitionTime)
					{
						auto const currentToPreviousScaleFactor = 1.f/animator.GetDurationScalingFactor(previousAnimator);
						auto const previousToCurrentScaleFactor = 1.f/previousAnimator.GetDurationScalingFactor(animator);
						
						auto const currentAnimatorSpeedVal = std::lerp(currentToPreviousScaleFactor, 1.f, transitionTime);
						animator.SetPlaybackSpeed(currentAnimatorSpeedVal);

						auto const previousAnimatorSpeedVal = std::lerp(1.f, previousToCurrentScaleFactor, transitionTime);
						previousAnimator.SetPlaybackSpeed(previousAnimatorSpeedVal);
					});
			}
		}
	}

	if(player_)
	{
		player_->UpdateAnimation(frame_time);
	}

	// build a transformation matrix that will position the character
	// use this to move the player around, scale it, etc.
	if (player_)
	{
		gef::Matrix44 player_transform;
		
		gef::Matrix44 player_scale;
		gef::Matrix44 player_translate;
		gef::Matrix44 player_rotate;

		player_transform.SetIdentity();
		player_scale.SetIdentity();
		player_translate.SetIdentity();
		player_rotate.SetIdentity();

		player_scale.Scale(gef::Vector4(0.3f,0.3f,0.3f,1));
		player_rotate.RotationY(gef::DegToRad(45.f));
		player_translate.SetTranslation(gef::Vector4(25.f, -25.f, -100.f, 1));

		player_transform = player_scale * player_rotate * player_translate;
		
		player_->SetTransform(player_transform);
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
		player_->RenderSelf(*renderer_3d_);
	

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
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT,
		                  "FPS: %.1f", fps_);
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

