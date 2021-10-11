// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "GravityAttractor.h"

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

		CollisionComponent->SetCollisionProfileName("OverlapAllDynamic");
		CollisionComponent->SetGenerateOverlapEvents(true);
	}
	if (!staticMeshComponent)
	{
		staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
		staticMeshComponent->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object);
		staticMeshComponent->SetWorldScale3D(FVector{ projectileSize });
		staticMeshComponent->SetupAttachment(RootComponent);
		staticMeshComponent->SetEnableGravity(false);
		staticMeshComponent->SetCollisionProfileName("NoCollision");
	}
	if (!gravityBody)
	{
		gravityBody = CreateDefaultSubobject<UGravityBody>("GravityBody");
	}
	InitialLifeSpan = lifeSpan; // Destroy projectile after X seconds
	SetReplicates(true);

	bAllowTickBeforeBeginPlay = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AProjectile::setPlanet(AActor * inPlanet, UGravityAttractor* inAttractor)
{
	gravityBody->setPlanetToOrbit(inPlanet, inAttractor);
	distanceFromPlanet = FVector::Distance(inPlanet->GetActorLocation(), GetActorLocation());
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Bind delegate
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::onBeginOverlap);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetGravityBody()->planet->GetActorLocation());
	AddActorLocalRotation(FRotator{ -rotationRate * DeltaTime, 0, 0 });
	SetActorLocation(GetActorLocation() + GetActorUpVector() * distanceFromPlanet);
}

void AProjectile::onBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GetLifeSpan() < lifeSpan - collisionDelay && Cast<APlayerCharacter>(OtherActor))
	{
		OtherActor->SetActorLocation(OtherActor->GetActorLocation() + OtherActor->GetActorUpVector() * 500.f);
		Cast<APlayerCharacter>(OtherActor)->playerScore++;
		Destroy();
	}
}
