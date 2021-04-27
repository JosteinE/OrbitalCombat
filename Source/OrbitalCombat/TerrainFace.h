// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ORBITALCOMBAT_API TerrainFace
{
public:
	TerrainFace(int resolution, FVector localUp, int section);
	~TerrainFace();

private:
	TArray<FVector> vertices;
	TArray<int> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uv0;

	void generateSectionForMesh(int resolution, FVector localUp, int section);

public:
	TArray<FVector> * getVertices();
	TArray<int> * getTriangles();
	TArray<FVector> * getNormals();
	TArray<FVector2D> * getUV0();
};
