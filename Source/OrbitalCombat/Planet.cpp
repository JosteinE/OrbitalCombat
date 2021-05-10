// Fill out your copyright notice in the Description page of Project Settings.

#include "Planet.h"
#include "TerrainFace.h"

// Sets default values
APlanet::APlanet()
{
	planetMesh = CreateDefaultSubobject<UProceduralMeshComponent>("PlanetMeshComponent");
	RootComponent = planetMesh;

	constructPlanet(resolution, spherical);
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlanet::constructPlanet(int res, bool bSphere)
{
	TArray<FVector> faceDirections{GetActorUpVector(), -GetActorUpVector(), 
								   GetActorRightVector(), -GetActorRightVector(),
								   GetActorForwardVector(), -GetActorForwardVector()};

	for (int section = 0; section < 6; section++) // 6 = number of faces/sections
	{
		TerrainFace terrainFace(res, faceDirections[section], section, bSphere);
		planetMesh->CreateMeshSection(section, *terrainFace.getVertices(), *terrainFace.getTriangles(),
		*terrainFace.getNormals(), *terrainFace.getUV0(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}
}

void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, resolution) || PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, spherical))
	{
		constructPlanet(resolution, spherical);
		RegisterAllComponents(); // Upon reconstruction, the component will have to be re-registered in order to be visible again
	}
}

