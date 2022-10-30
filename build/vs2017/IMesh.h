#pragma once

#include "IProtectedWrapper.h"
#include "graphics/mesh.h"
#include "system/string_id.h"

namespace AnimationSystem
{
	typedef IProtectedWrapper<gef::Mesh> IMesh;

	// class GefMeshWrapper final :IMesh
	// {
	// public:
	// 	static std::unique_ptr<IMesh> Create(std::unique_ptr<gef::Mesh> mesh, const gef::StringId id)
	// 	{
	// 		auto iMesh = std::unique_ptr<IMesh>{new GefMeshWrapper{std::move(mesh), id}};
	// 		return iMesh;
	// 	}
	//
	// 	gef::Mesh const & Item() const override {return *mesh_;}
	// 	unsigned ID() const override {return id_;}
	// 	
	// private:
	// 	// prevent creation outside of class
	// 	explicit GefMeshWrapper(std::unique_ptr<gef::Mesh> mesh, const gef::StringId id)
	// 		:mesh_(std::move(mesh))
	// 		,id_(id)
	// 	{
	// 	}
	//
	// 	std::unique_ptr<gef::Mesh> mesh_;
	// 	const gef::StringId id_;
	//
	// 	void operator delete(void * p){::operator delete(p);}
	// };
}