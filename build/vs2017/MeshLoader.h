#pragma once
#include <memory>
#include <string>
#include <utility>

#include "IMesh.h"
#include "IMeshLoader.h"
#include "Result.h"

using std::string;


namespace AnimationSystem
{
	class MeshLoader final : IMeshLoader
	{
	public:
		Result LoadMesh(const std::string& filepath) override;
		const std::unique_ptr<IMesh>& GetMesh(string name) override;

	private:

		const std::unique_ptr<IMesh>& null_pointer_ = nullptr;
	};
}
