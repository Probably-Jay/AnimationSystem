#pragma once
#include <memory>
#include <string>

#include "MeshWrapper.h"
#include "PureResult.h"
#include "graphics/scene.h"


namespace AnimationSystem
{
	class IMeshLoader
	{
	public:
		virtual ~IMeshLoader() = default;
		virtual AnimationSystem::PureResult LoadMeshScene(gef::Scene& scene) = 0;
		virtual MeshWrapper const * GetMesh(std::string const& name) const = 0;
		virtual MeshWrapper const * GetMesh(unsigned id) const = 0;
		virtual std::vector<unsigned int> GetAllMeshIDs() const = 0;
	};
}
