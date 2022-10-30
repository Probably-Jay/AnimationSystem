#pragma once

#include "graphics/mesh.h"

namespace AnimationSystem
{
	class IMesh
	{
	public:
		virtual ~IMesh() = default;

		// todo remove dependency
		virtual gef::Mesh & Mesh() const = 0;
	protected:
		IMesh() = default; // prevent creation outside of class
	
		private:
		void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

		friend struct std::default_delete<IMesh>; // allow deletion by smart pointer
	};
}
namespace AnimationSystem
{
	class GefMeshWrapper : IMesh
	{
	public:
		static std::unique_ptr<IMesh> Create(std::unique_ptr<gef::Mesh> mesh)
		{
			auto iMesh = std::unique_ptr<IMesh>{new GefMeshWrapper{std::move(mesh)}};
			return iMesh;
		}

		gef::Mesh & Mesh() const override {return *mesh_;}

		~GefMeshWrapper() override = default;	
	private:
		
		// prevent creation outside of class
		explicit GefMeshWrapper(std::unique_ptr<gef::Mesh> mesh)
			:IMesh()
			,mesh_(std::move(mesh))
		{
		}

		std::unique_ptr<gef::Mesh> mesh_;

		void operator delete(void * p){::operator delete(p);}
	};
}