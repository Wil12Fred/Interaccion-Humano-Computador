#pragma once
// Iostream - STD I/O Library
#include <iostream>
// Vector - STD Vector/Array Library
#include <vector>
// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace objl
{

	// Structure: Mesh
	//
	// Description: A Simple Mesh Object that holds
	//	a name, a vertex list, and an index list
	struct Mesh
	{
		// Default Constructor
		Mesh()
		{

		}
		// Variable Set Constructor
		Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
		{
			Vertices = _Vertices;
			Indices = _Indices;
		}
		// Mesh Name
		std::string MeshName;
		// Vertex List
		std::vector<Vertex> Vertices;
		// Index List
		std::vector<unsigned int> Indices;
		// Material
		Vector3 defaultColor;
		Material MeshMaterial;
	};

}
