#pragma once
#include <memory>
#include <string>

#include "PureResult.h"
#include "graphics/scene.h"


namespace AnimationSystem
{
	// class IMeshLoader
	// {
	// public:
	// 	virtual ~IMeshLoader() = default;
	// 	virtual AnimationSystem::PureResult LoadMeshScene(gef::Scene& scene) = 0;
	// 	[[nodiscard]] virtual std::optional<std::reference_wrapper<const gef::Mesh>> GetMesh(std::string const& name) const = 0;
	// 	[[nodiscard]] virtual std::optional<std::reference_wrapper<const gef::Mesh>> GetMesh(unsigned id) const = 0;
	// 	[[nodiscard]] virtual std::vector<unsigned int> GetAllMeshIDs() const = 0;
	// };
}
