#pragma once


class IMesh
{
public:
	virtual ~IMesh() = 0; // pure abstract
	
protected:
	IMesh() = default; // prevent creation outside of class
	
private:
	void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

	friend struct std::default_delete<IMesh>; // allow deletion by smart pointer
};


class Mesh : IMesh
{
public:
	static std::unique_ptr<IMesh> Create()
	{
		std::unique_ptr<IMesh> iMesh {new Mesh{}};
		return iMesh;
	}

private:
	Mesh() = default; // prevent creation outside of class
};

