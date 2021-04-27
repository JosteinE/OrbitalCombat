// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ORBITALCOMBAT_API TerrainFace
{
public:
	TerrainFace(int resolution, FVector localUp, int section);
	~TerrainFace();

private:
	TArray<FVector> * vertices;
	TArray<int> * triangles;

	void generateSectionForMesh(int resolution, FVector localUp, int section);

public:
	TArray<FVector> * getVertices();
	TArray<int> * getTriangles();
};
