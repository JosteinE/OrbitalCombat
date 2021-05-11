// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h" // To include this i had to: add it to .Build.cs then generate files using .Uproject
#include "Planet.generated.h"

UCLASS()
class ORBITALCOMBAT_API APlanet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UProceduralMeshComponent * planetMesh;

	UPROPERTY(EditAnywhere, meta=(ClampMin = "2", ClampMax = "256"))
	int resolution = 12;

	UPROPERTY(EditAnywhere)
	bool spherical = true;

private:
	void constructPlanet(int res, bool bSphere = true);
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
