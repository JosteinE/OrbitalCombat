// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityAttractor.h"
#include "GameFramework/Actor.h"
#include "GravityBody.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBITALCOMBAT_API UGravityBody : public UActorComponent
{
	GENERATED_BODY()

protected:
	UGravityBody();
	~UGravityBody();

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor *planet {nullptr}; // Must have the GravityAttractor component for this to work!

	UGravityAttractor* planetAttractor{ nullptr };

	void setPlanetToOrbit(AActor *inPlanet, UGravityAttractor* gravityAttractor);
	void rotateToSurface();
	void pullToSurface(float deltaTime, bool * bGrounded = nullptr);
	void rotateMeshToSurface();
	void pullMeshToSurface(float deltaTime, bool * bGrounded = nullptr); //Applies gravity
};
