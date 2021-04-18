// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityAttractor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

UGravityAttractor::UGravityAttractor()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UGravityAttractor::RotateToSurface(AActor* body)
{
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), body->GetActorLocation());
	FVector newForwardVector = UKismetMathLibrary::GetForwardVector(newRotation);
	UKismetMathLibrary::MakeRotFromZX(newForwardVector, body->GetActorForwardVector());

	body->SetActorRotation(UKismetMathLibrary::MakeRotFromZX(newForwardVector, body->GetActorForwardVector()));
}

void UGravityAttractor::RotateMeshToSurface(UStaticMeshComponent * mesh)
{
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), mesh->GetComponentLocation());
	FVector newForwardVector = UKismetMathLibrary::GetForwardVector(newRotation);
	UKismetMathLibrary::MakeRotFromZX(newForwardVector, mesh->GetForwardVector());

	mesh->SetRelativeRotation(UKismetMathLibrary::MakeRotFromZX(newForwardVector, mesh->GetForwardVector()));
}

void UGravityAttractor::Attract(AActor * body, float deltaTime, bool *bGrounded)
{
	FVector pullDirection{ -body->GetActorUpVector() };
	pullDirection.Normalize();

	if (bGrounded != nullptr)
	{
		FHitResult bColliding; // To log the hit result
		body->SetActorLocation(body->GetActorLocation() + pullDirection * gravity, true, &bColliding);

		if (bColliding.bBlockingHit)
		{
			*bGrounded = true;
		}
		else
		{
			*bGrounded = false;
		}
	}
	else
	{
		body->SetActorLocation(body->GetActorLocation() + pullDirection * gravity, true);
	}
}

void UGravityAttractor::AttractMesh(UStaticMeshComponent * mesh, float deltaTime, bool *bGrounded)
{
	FVector pullDirection{ -mesh->GetUpVector() };
	pullDirection.Normalize();
	
	if (bGrounded != nullptr)
	{
		FHitResult bColliding; // To log the hit result
		mesh->SetWorldLocation(mesh->GetComponentLocation() + pullDirection * gravity, true, &bColliding);

	/*	if (mesh->GetPhysicsAngularVelocity().GetMax() > 0.001f)
		{
			mesh->PutRigidBodyToSleep();
			UE_LOG(LogTemp, Warning, TEXT("Body put to sleep"));
		}

		UE_LOG(LogTemp, Warning, TEXT("mesh velocity %f"), mesh->GetPhysicsAngularVelocity().GetMax());*/

		if (bColliding.bBlockingHit)
		{
			*bGrounded = true;
			//UE_LOG(LogTemp, Warning, TEXT("Colliding with: %s"), *bColliding.GetActor()->GetName());
		}
		else
		{
			*bGrounded = false;
			//UE_LOG(LogTemp, Warning, TEXT("NOT Colliding!"));
		}
	}
	else
	{
		mesh->SetWorldLocation(mesh->GetComponentLocation() + pullDirection * gravity, true);
	}
}