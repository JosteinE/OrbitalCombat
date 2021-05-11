// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ORBITALCOMBAT_API TerrainFace
{
public:
	TerrainFace(int resolution, FVector localUp, int section, bool bSphere = true);
	~TerrainFace();

private:
	TArray<FVector> vertices;
	TArray<int> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uv0;

public:
	TArray<FVector> * getVertices();
	TArray<int> * getTriangles();
	TArray<FVector> * getNormals();
	TArray<FVector2D> * getUV0();
};
