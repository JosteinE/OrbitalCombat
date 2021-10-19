// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityAttractor.generated.h"

class StaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBITALCOMBAT_API UGravityAttractor : public UActorComponent
{
	GENERATED_BODY()

protected:
	UGravityAttractor();

public:

	UPROPERTY(EditAnywhere)
	float gravity = 5.f;

	FRotator GetRotateToSurface(AActor* body);
	void RotateMeshToSurface(UStaticMeshComponent * mesh);
	FVector GetAttract(AActor * body, float deltaTime);
	void AttractMesh(UStaticMeshComponent * mesh, float deltaTime, bool *bGrounded = nullptr);
};