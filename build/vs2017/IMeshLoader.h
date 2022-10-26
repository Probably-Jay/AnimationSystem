#pragma once
#include <memory>
#include <string>

#include "IMesh.h"
#include "Result.h"


class IMeshLoader
{
public:
	virtual Result LoadMesh(std::string const & filepath) = 0;
	virtual const std::unique_ptr<IMesh>& GetMesh(std::string name) = 0;
	virtual ~IMeshLoader() = default;
protected:
	//IMeshLoader() = default;
	
};
