// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrainFace.h"

TerrainFace::TerrainFace(int resolution, FVector localUp, int section, bool bSphere)
{
	FVector axisA{ localUp.Z, localUp.X, localUp.Y }; // A scrambled version of the up vector
	FVector axisB{ FVector::CrossProduct(localUp, axisA) }; // A vector that is perpendicular to localUp and axisA
	// Number of Squares	= (resolution - 1)^2
	// Number of Triangles	= (resolution - 1)^2 * 2			
	// Number of Vertices	= (resolution - 1)^2 * 2 * 3
	vertices.AddZeroed(resolution * resolution); // = new TArray<FVector>[resolution * resolution];
	triangles.AddZeroed((resolution - 1)*(resolution - 1) * 2 * 3); // = new TArray<int32>[(resolution - 1)*(resolution - 1) * 2 * 3];
	normals.AddZeroed(resolution * resolution);// = new TArray<FVector>[resolution * resolution];
	uv0.AddZeroed(resolution * resolution); // = new TArray<FVector2D>[resolution * resolution];
	int triIndex = 0;
	int index = 0;
	float meshStart = -50.f;
	float meshEnd = -meshStart;
	float vertStep = (meshEnd - meshStart) / (resolution - 1.f);
	for (float y = meshStart; y <= meshEnd; y += vertStep)		// We add the step amount here for the y axis
	{
		for (float x = meshStart; x <= meshEnd; x += vertStep)	// We add the step amount here for the x axis
		{
			FVector pointOnUnitCube = localUp * meshEnd + axisA * x + axisB * y; // Starting at x = meshStart, y = meshStart, z = Up * meshEnd, constructing along the x axis before moving down y.
			if (bSphere)
			{
				pointOnUnitCube.Normalize();
				pointOnUnitCube *= meshEnd;
			}
			vertices[index] = pointOnUnitCube;
			normals[index] = localUp;
			uv0[index] = FVector2D{ x, y };

			// Constructing the triangles counter-clockwise, making sure we're not going constructing triangles beyond scope
			if (x <= meshEnd - vertStep && y <= meshEnd - vertStep && triIndex < triangles.Num()) // shouldnt really need this last num check...
			{
				triangles[triIndex] = index;
				triangles[triIndex + 1] = index + resolution;
				triangles[triIndex + 2] = index + resolution + 1;

				triangles[triIndex + 3] = index;
				triangles[triIndex + 4] = index + resolution + 1;
				triangles[triIndex + 5] = index + 1;
				triIndex += 6;
			}
			index++;

		}
	}
}

TerrainFace::~TerrainFace()
{
	//vertices->Empty();
	//delete vertices;

	//triangles->Empty();
	//delete triangles;

	//normals->Empty();
	//delete normals;

	//uv0->Empty();
	//delete normals;
}

TArray<FVector>* TerrainFace::getVertices()
{
	return &vertices;
}

TArray<int>* TerrainFace::getTriangles()
{
	return &triangles;
}

TArray<FVector>* TerrainFace::getNormals()
{
	return &normals;
}

TArray<FVector2D>* TerrainFace::getUV0()
{
	return &uv0;
}
