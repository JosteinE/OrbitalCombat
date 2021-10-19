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

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_RotateToSurface();
	bool Client_RotateToSurface_Validate();
	void Client_RotateToSurface_Implementation();

	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//void Multi_RotateToSurface();
	//bool Multi_RotateToSurface_Validate();
	//void Multi_RotateToSurface_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_PullToSurface(float deltaTime);
	bool Client_PullToSurface_Validate(float deltaTime);
	void Client_PullToSurface_Implementation(float deltaTime);

	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//void Multi_PullToSurface(float deltaTime);
	//bool Multi_PullToSurface_Validate(float deltaTime);
	//void Multi_PullToSurface_Implementation(float deltaTime);




	void rotateMeshToSurface();
	void pullMeshToSurface(float deltaTime, bool * bGrounded = nullptr); //Applies gravity
};
