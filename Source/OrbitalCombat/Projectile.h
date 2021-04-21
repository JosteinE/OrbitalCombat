// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "OrbitalProjectileMovementComp.h"
#include "GravityBody.h"
#include "Projectile.generated.h"

UCLASS()
class ORBITALCOMBAT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float distanceFromPlanet;

	/** Returns the movementComponent subobject **/
	FORCEINLINE class UOrbitalProjectileMovementComp* GetMovementComponent() { return projectileMovementComponent; }
	/** Returns the gravityBody subobject **/
	FORCEINLINE class UGravityBody* GetGravityBody() { return gravityBody; }

	UPROPERTY(EditAnywhere)
	float projectileSpeed = 100.f;

	UPROPERTY(EditAnywhere)
	float projectileSize = 0.5f;

	UPROPERTY(EditAnywhere)
	float lifeSpan = 4.0f;

	// Sphere collision component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UOrbitalProjectileMovementComp* projectileMovementComponent;

	// Mesh Component
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* staticMeshComponent;

	/** Gravity body - Rotates (and pulls) owner towards a target */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGravityBody *gravityBody;

	FVector projectileDirection;
	// Function that initializes the projectile's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);
};
