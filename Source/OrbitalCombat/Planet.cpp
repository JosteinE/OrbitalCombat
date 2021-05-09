// Fill out your copyright notice in the Description page of Project Settings.

#include "Planet.h"
#include "TerrainFace.h"

// Sets default values
APlanet::APlanet()
{
	planetMesh = CreateDefaultSubobject<UProceduralMeshComponent>("PlanetMeshComponent");
	RootComponent = planetMesh;

	constructPlanet();
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlanet::constructPlanet()
{
	TArray<FVector> faceDirections{GetActorUpVector(), -GetActorUpVector(), 
								   GetActorRightVector(), -GetActorRightVector(),
								   GetActorForwardVector(), -GetActorForwardVector()};

	for (int section = 0; section < 6; section++) // 6 = number of faces/sections
	{
		planetMesh->ClearMeshSection(section);

		TerrainFace terrainFace(resolution, faceDirections[section], section);
		planetMesh->CreateMeshSection(section, *terrainFace.getVertices(), *terrainFace.getTriangles(),
		*terrainFace.getNormals(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}
}

