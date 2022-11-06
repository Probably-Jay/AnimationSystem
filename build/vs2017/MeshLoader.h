#pragma once
#include <memory>
#include <string>

#include "IMeshLoader.h"
#include "PureResult.h"
#include "graphics/scene.h"


using std::string;
using std::unique_ptr;

namespace AnimationSystem
{
	class MeshLoader
	{
	public:
		PureResult LoadMesh(gef::StringId id, gef::Scene& scene, gef::Platform& platform);
		[[nodiscard]] gef::Mesh & Mesh() const {return mesh_.value().get();}
	private:
		std::optional<std::reference_wrapper<gef::Mesh>> mesh_ ={};
	};
}
