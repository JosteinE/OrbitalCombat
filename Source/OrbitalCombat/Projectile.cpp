// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!projectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		projectileMovementComponent = CreateDefaultSubobject<UOrbitalProjectileMovementComp>(TEXT("ProjectileMovementComponent"));
		//projectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		projectileMovementComponent->InitialSpeed = 600.0f;
		projectileMovementComponent->MaxSpeed = 600.0f;
		projectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	if (!staticMeshComponent)
	{
		staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
		staticMeshComponent->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object);
		staticMeshComponent->SetupAttachment(RootComponent);
		staticMeshComponent->SetEnableGravity(false);
	}
	if (!gravityBody)
		gravityBody = CreateDefaultSubobject<UGravityBody>("GravityBody");
	

	InitialLifeSpan = 3.0f; // Destroy projectile after X seconds
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate mesh to face surface
	GetGravityBody()->rotateToSurface();

	// Pull mesh towards the main planet, also detecting collision
	GetGravityBody()->pullToSurface(DeltaTime);
}

// Function that initializes the projectile's velocity in the shoot direction.
void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	projectileDirection = ShootDirection;
	projectileDirection.Normalize();
	//projectileMovementComponent->Velocity = ShootDirection * projectileMovementComponent->InitialSpeed;
}
