// Fill out your copyright notice in the Description page of Project Settings.

#include "Planet.h"
#include "TerrainFace.h"

// Sets default values
APlanet::APlanet()
{
	planetMesh = CreateDefaultSubobject<UProceduralMeshComponent>("PlanetMeshComponent");
	RootComponent = planetMesh;
	planetMesh->SetWorldScale3D(FVector{ 100,100,100 });

	constructPlanet(resolution);
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlanet::constructPlanet(int res)
{
	TArray<FVector> faceDirections{GetActorUpVector(), -GetActorUpVector(), 
								   GetActorRightVector(), -GetActorRightVector(),
								   GetActorForwardVector(), -GetActorForwardVector()};

	for (int section = 0; section < 6; section++) // 6 = number of faces/sections
	{
		TerrainFace terrainFace(res, faceDirections[section], section);
		planetMesh->CreateMeshSection(section, *terrainFace.getVertices(), *terrainFace.getTriangles(),
		*terrainFace.getNormals(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}
}

void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, resolution))
	{
		constructPlanet(resolution);
		RegisterAllComponents(); // Upon reconstruction, the component will have to be re-registered in order to be visible again
		UE_LOG(LogTemp, Warning, TEXT("Planet reconstructed with a resolution of %i"), resolution);
	}
}

