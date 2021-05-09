// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrainFace.h"

TerrainFace::TerrainFace(int resolution, FVector localUp, int section)
{
	FVector axisA{ localUp.Y, localUp.Z, localUp.X }; // A vector pointing in a 90degree angle from localUp
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
	float meshStart = -1.f;
	float meshEnd = 1.f;
	float vertStep = (meshEnd - meshStart) / (resolution - 1.f);
	for (float y = meshStart; y <= meshEnd; y += vertStep)		// We add the step amount here for the y axis
	{
		for (float x = meshStart; x <= meshEnd; x += vertStep)	// We add the step amount here for the x axis
		{
			FVector pointOnUnitCube = localUp + axisA * x + axisB * y; // Starting at x = -0.5, y = -0.5, z = Up, constructing along the x axis before moving down y.
			vertices[index] = pointOnUnitCube;
			normals[index] = localUp;

			// Constructing the triangles, making sure we're not going constructing triangles beyond scope
			if (x < meshEnd && y < meshEnd && triIndex < triangles.Num())
			{
				triangles[triIndex] = index;
				triangles[triIndex + 1] = index + resolution;
				triangles[triIndex + 2] = index + resolution + 1;

				triangles[triIndex + 3] = index;
				triangles[triIndex + 4] = index + resolution + 1;
				triangles[triIndex + 5] = index + 1;
				//triangles[triIndex] = index;
				//triangles[triIndex + 1] = index + resolution + 1;
				//triangles[triIndex + 2] = index + resolution;

				//triangles[triIndex + 3] = index;
				//triangles[triIndex + 4] = index + 1;
				//triangles[triIndex + 5] = index + resolution + 1;
				triIndex += 6;
			}
			index++;

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("triIndex: %i"), triIndex);
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

void TerrainFace::generateSectionForMesh(int resolution, FVector localUp, int section)
{
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
