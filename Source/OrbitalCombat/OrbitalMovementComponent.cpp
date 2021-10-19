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

	if (GetWorld()->IsServer())
	{
		RotateToSurface();			// Rotate mesh to face surface
		PullToSurface(DeltaTime);	// Pull mesh towards the main planet, also detecting collision
	}
	else
	{
		Server_RotateToSurface();
		Multi_RotateToSurface();

		Server_PullToSurface(DeltaTime);
		Multi_PullToSurface(DeltaTime);
	}

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
		MoveCharacter(moveDirection, moveSpeed, deltaTime);
	}
	else
	{
		// Remote client moves and sends the transformation to the server
		Server_MoveCharacter(moveDirection, moveSpeed, deltaTime);
		// Broadcasted movement to all remote clients
		Multi_MoveCharacter(moveDirection, moveSpeed, deltaTime);
	}
}

void UOrbitalMovementComponent::MoveCharacter(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	GetPawnOwner()->SetActorLocation(GetPawnOwner()->GetActorLocation() + moveDirection * inMoveSpeed * deltaTime, false);
}

void UOrbitalMovementComponent::RotateToSurface()
{
	GetPawnOwner()->SetActorRotation(Cast<APlayerCharacter>(GetPawnOwner())->GetGravityBody()->rotateToSurface());
}

void UOrbitalMovementComponent::PullToSurface(float deltaTime)
{
	FHitResult bColliding; // To log the hit result
	GetPawnOwner()->SetActorLocation(Cast<APlayerCharacter>(GetPawnOwner())->GetGravityBody()->pullToSurface(deltaTime), true, &bColliding);
	bGrounded = bColliding.bBlockingHit;
}

bool UOrbitalMovementComponent::Server_MoveCharacter_Validate(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	return true;
}

void UOrbitalMovementComponent::Server_MoveCharacter_Implementation(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	MoveCharacter(moveDirection, moveSpeed, deltaTime);
}

bool UOrbitalMovementComponent::Multi_MoveCharacter_Validate(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	return true;
}

void UOrbitalMovementComponent::Multi_MoveCharacter_Implementation(FVector moveDirection, float inMoveSpeed, float deltaTime)
{
	MoveCharacter(moveDirection, moveSpeed, deltaTime);
}

bool UOrbitalMovementComponent::Server_RotateToSurface_Validate()
{
	return true;
}

void UOrbitalMovementComponent::Server_RotateToSurface_Implementation()
{
	RotateToSurface();
}

bool UOrbitalMovementComponent::Multi_RotateToSurface_Validate()
{
	return true;
}

void UOrbitalMovementComponent::Multi_RotateToSurface_Implementation()
{
	RotateToSurface();
}

bool UOrbitalMovementComponent::Server_PullToSurface_Validate(float deltaTime)
{
	return true;
}

void UOrbitalMovementComponent::Server_PullToSurface_Implementation(float deltaTime)
{
	PullToSurface(deltaTime);
}

bool UOrbitalMovementComponent::Multi_PullToSurface_Validate(float deltaTime)
{
	return true;
}

void UOrbitalMovementComponent::Multi_PullToSurface_Implementation(float deltaTime)
{
	PullToSurface(deltaTime);
}

void UOrbitalMovementComponent::jump()
{
	if (bGrounded)
	{
		//Cast<APlayerCharacter>(GetPawnOwner())->GetMeshComponent()->SetPhysicsLinearVelocity(Cast<APlayerCharacter>(GetPawnOwner())->GetMeshComponent()->GetUpVector() * jumpForce, true);
		//UE_LOG(LogTemp, Warning, TEXT("GetUpVector: { %f, %f, %f }"), meshComponent->GetUpVector().X, meshComponent->GetUpVector().Y, meshComponent->GetUpVector().Z);
	}
}

