// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbitalMovementComponent.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UOrbitalMovementComponent::UOrbitalMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
	bTickBeforeOwner = false;
}


// Called when the game starts
void UOrbitalMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UOrbitalMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Rotate mesh to face surface
	Cast<APlayerCharacter>(GetPawnOwner())->GetGravityBody()->rotateToSurface();
	// Pull mesh towards the main planet, also detecting collision
	Cast<APlayerCharacter>(GetPawnOwner())->GetGravityBody()->pullToSurface(DeltaTime, &bGrounded);
}

void UOrbitalMovementComponent::inputToMovement(FVector input, bool bJumping, float deltaTime)
{
	FVector moveDirection;
	if (input.X != 0)
		moveDirection += GetPawnOwner()->GetActorRightVector() * input.X;

	if (input.Y != 0)
		moveDirection += GetPawnOwner()->GetActorForwardVector() * input.Y;

	moveDirection.Normalize();

	//UE_LOG(LogTemp, Warning, TEXT("MoveDirection { %f, %f, %f }"), moveDirection.X, moveDirection.Y, moveDirection.Z);
	//GetPawnOwner()->AddMovementInput(moveDirection, moveSpeed, true);
	
	if (GetWorld()->IsServer())
	{
		// Host moves
		MoveCharacter(&moveDirection, &moveSpeed, &deltaTime);
	}
	else
	{
		// Remote client moves and sends the transformation to the server
		Server_MoveCharacter(moveDirection, moveSpeed, deltaTime);
		// Broadcasted movement to all remote clients
		Multi_MoveCharacter(moveDirection, moveSpeed, deltaTime);
	}
}

void UOrbitalMovementComponent::MoveCharacter(FVector * moveDirection, float * inMoveSpeed, float * deltaTime)
{
	GetPawnOwner()->SetActorLocation(GetPawnOwner()->GetActorLocation() + (*moveDirection) * (*inMoveSpeed) * (*deltaTime), false);
}

bool UOrbitalMovementComponent::Server_MoveCharacter_Validate(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	return true;
}

void UOrbitalMovementComponent::Server_MoveCharacter_Implementation(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	MoveCharacter(&moveDirection, &moveSpeed, &deltaTime);
}

bool UOrbitalMovementComponent::Multi_MoveCharacter_Validate(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	return true;
}

void UOrbitalMovementComponent::Multi_MoveCharacter_Implementation(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	MoveCharacter(&moveDirection, &moveSpeed, &deltaTime);
}

void UOrbitalMovementComponent::jump()
{
	if (bGrounded)
	{
		//Cast<APlayerCharacter>(GetPawnOwner())->GetMeshComponent()->SetPhysicsLinearVelocity(Cast<APlayerCharacter>(GetPawnOwner())->GetMeshComponent()->GetUpVector() * jumpForce, true);
		//UE_LOG(LogTemp, Warning, TEXT("GetUpVector: { %f, %f, %f }"), meshComponent->GetUpVector().X, meshComponent->GetUpVector().Y, meshComponent->GetUpVector().Z);
	}
}

