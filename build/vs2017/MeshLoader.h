#pragma once
#include <memory>
#include <string>

#include "IMesh.h"
#include "IMeshLoader.h"
#include "Result.h"
#include "graphics/scene.h"

using std::string;

namespace AnimationSystem
{
	class MeshLoader final : public IMeshLoader
	{
	public:
		explicit MeshLoader(gef::Platform& platform_);
		Result LoadMeshScene(const std::string& filepath) override;
		IMesh const * GetMesh(string name) override;
	private:

		const std::unique_ptr<IMesh>& null_pointer_ = nullptr;
		gef::Platform& platform_
		gef::Scene model_scene_;
	};
}
