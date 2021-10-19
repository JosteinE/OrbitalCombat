// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityAttractor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

UGravityAttractor::UGravityAttractor()
{
	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bStartWithTickEnabled = true;
}

FRotator UGravityAttractor::GetRotateToSurface(AActor* body)
{
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), body->GetActorLocation());
	FVector newForwardVector = UKismetMathLibrary::GetForwardVector(newRotation);
	return UKismetMathLibrary::MakeRotFromZX(newForwardVector, body->GetActorForwardVector());
}

void UGravityAttractor::RotateMeshToSurface(UStaticMeshComponent * mesh)
{
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), mesh->GetComponentLocation());
	FVector newForwardVector = UKismetMathLibrary::GetForwardVector(newRotation);
	UKismetMathLibrary::MakeRotFromZX(newForwardVector, mesh->GetForwardVector());

	mesh->SetRelativeRotation(UKismetMathLibrary::MakeRotFromZX(newForwardVector, mesh->GetForwardVector()));
}

FVector UGravityAttractor::GetAttract(AActor * body, float deltaTime)
{
	FVector pullDirection{ -body->GetActorUpVector() };
	pullDirection.Normalize();
	return body->GetActorLocation() + pullDirection * gravity;
}

void UGravityAttractor::AttractMesh(UStaticMeshComponent * mesh, float deltaTime, bool *bGrounded)
{
	FVector pullDirection{ -mesh->GetUpVector() };
	pullDirection.Normalize();
	
	if (bGrounded != nullptr)
	{
		FHitResult bColliding; // To log the hit result
		mesh->SetWorldLocation(mesh->GetComponentLocation() + pullDirection * gravity, true, &bColliding);

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
