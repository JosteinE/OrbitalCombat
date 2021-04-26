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
		CollisionComponent->InitSphereRadius(projectileSize);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!staticMeshComponent)
	{
		staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
		staticMeshComponent->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object);
		staticMeshComponent->SetWorldScale3D(FVector{ projectileSize });
		staticMeshComponent->SetupAttachment(RootComponent);
		staticMeshComponent->SetEnableGravity(false);
	}
	if (!gravityBody)
	{
		gravityBody = CreateDefaultSubobject<UGravityBody>("GravityBody");
	}
	
	InitialLifeSpan = lifeSpan; // Destroy projectile after X seconds
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	distanceFromPlanet = FVector::Distance(GetGravityBody()->planet->GetActorLocation(), GetActorLocation());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetGravityBody()->planet->GetActorLocation());
	AddActorLocalRotation(FRotator{ -rotationRate * DeltaTime, 0, 0 });
	SetActorLocation(GetActorLocation() + GetActorUpVector() * distanceFromPlanet, true);
}
