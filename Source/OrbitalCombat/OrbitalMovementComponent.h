// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "OrbitalMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBITALCOMBAT_API UOrbitalMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOrbitalMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	float moveSpeed = 600.f;

	UPROPERTY(EditAnywhere)
	float jumpForce = 1500.f;

	bool bGrounded;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void inputToMovement(FVector input, bool bJumping, float deltaTime);

	void MoveCharacter(FVector* moveDirection, float* inMoveSpeed, float* deltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveCharacter(FVector moveDirection, float inMoveSpeed, float deltaTime);
	bool Server_MoveCharacter_Validate(FVector moveDirection, float inMoveSpeed, float deltaTime);
	void Server_MoveCharacter_Implementation(FVector moveDirection, float inMoveSpeed, float deltaTime);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_MoveCharacter(FVector moveDirection, float inMoveSpeed, float deltaTime);
	bool Multi_MoveCharacter_Validate(FVector moveDirection, float inMoveSpeed, float deltaTime);
	void Multi_MoveCharacter_Implementation(FVector moveDirection, float inMoveSpeed, float deltaTime);

	void jump();
};