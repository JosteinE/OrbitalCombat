// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrainFace.h"

TerrainFace::TerrainFace(int resolution, FVector localUp, int section)
{
	FVector axisA{ localUp.Y, localUp.Z, localUp.X }; // A vector pointing in a 90degree angle from localUp
	FVector axisb{ FVector::CrossProduct(localUp, axisA) }; // A vector that is perpendicular to localUp and axisA

	// Number of Squares	= (resolution - 1)^2
	// Number of Triangles	= (resolution - 1)^2 * 2			
	// Number of Vertices	= (resolution - 1)^2 * 2 * 3
	vertices = new TArray<FVector>[resolution * resolution];
	triangles = new TArray<int>[(resolution - 1)*(resolution - 1) * 2 * 3];
	int triIndex = 0;

	for (int y = -1; y < resolution; y += 1 / (resolution * 0.5f))		// We add the step amount here for the y axis
	{
		for (int x = -1; x < resolution; x += 1 / (resolution * 0.5f))	// We add the step amount here for the x axis
		{
			FVector pointOnUnitCube = localUp + axisA * x + axisb * y; // Starting at x = -1, y = -1, z = Up, constructing along the x axis before moving down y.
			int index = (x + y + 2) * (resolution * 0.5f); // Current index based on number of iterations calculated by using the sum of x and y
			(*vertices)[index] = pointOnUnitCube;

			// Constructing the triangles, making sure we're not going constructing triangles beyond scope
			if (x != resolution - (1 / (resolution * 0.5f)) && y != resolution - (1 / (resolution * 0.5f)))
			{
				(*triangles)[triIndex] = index;
				(*triangles)[triIndex + 1] = index + resolution + 1;
				(*triangles)[triIndex + 2] = index + resolution;

				(*triangles)[triIndex + 3] = index;
				(*triangles)[triIndex + 4] = index + 1;
				(*triangles)[triIndex + 5] = index + resolution;
				triIndex += 6;
			}
		}
	}
}

TerrainFace::~TerrainFace()
{
	vertices->Empty();
	delete vertices;

	triangles->Empty();
	delete triangles;
}

void TerrainFace::generateSectionForMesh(int resolution, FVector localUp, int section)
{
}

TArray<FVector>* TerrainFace::getVertices()
{
	return vertices;
}

TArray<int>* TerrainFace::getTriangles()
{
	return triangles;
}
