#pragma once
#include <memory>
#include <string>

#include "IMesh.h"
#include "Result.h"


namespace AnimationSystem
{
	class IMeshLoader
	{
	public:
		virtual ~IMeshLoader() = default;
		virtual Result LoadMeshScene(std::string const & filepath) = 0;
		virtual IMesh const * GetMesh(std::string const& name) = 0;	
	};
}