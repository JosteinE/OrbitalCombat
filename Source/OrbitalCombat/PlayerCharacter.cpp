#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "GameFramework/PawnMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// Set size for player capsule
	playerCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	playerCapsuleComponent->InitCapsuleSize(25.f, 50.0f);
	playerCapsuleComponent->SetEnableGravity(false);
	playerCapsuleComponent->SetCollisionProfileName("Pawn");
	playerCapsuleComponent->SetGenerateOverlapEvents(true);
	RootComponent = playerCapsuleComponent;

	// Create a mesh for the player
	skeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	skeletalMeshComponent->SetEnableGravity(false);
	skeletalMeshComponent->SetupAttachment(RootComponent);

	// Create a movement component for the pawn
	movementComponent = CreateDefaultSubobject<UOrbitalMovementComponent>("MovementComponent");

	// Create a camera boom...
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 1500.f;
	cameraBoom->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	cameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	playerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	playerCameraComponent->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);

	//// Create a decal in the world to show the cursor's location
	//cursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	//cursorToWorld->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	//if (DecalMaterialAsset.Succeeded())
	//{
	//	cursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	//}
	//cursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	//cursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Add the cursor plane mesh
	cursorPlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>("cursorPlaneMeshComponent");
	cursorPlaneMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object);
	cursorPlaneMesh->SetWorldScale3D(FVector{ 15.f, 30.f, 1.f });
	cursorPlaneMesh->SetCollisionProfileName("CursorCollider");
	cursorPlaneMesh->SetEnableGravity(false);
	cursorPlaneMesh->SetCastShadow(false);
	cursorPlaneMesh->SetupAttachment(RootComponent);

	// Add the Gravity Body
	gravityBody = CreateDefaultSubobject<UGravityBody>("GravityBody");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if (cursorToWorld != nullptr)
	//{
	//	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	//	{
	//		FHitResult TraceHitResult;
	//		PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	//		FVector CursorFV = TraceHitResult.ImpactNormal;
	//		FRotator CursorR = CursorFV.Rotation();
	//		cursorToWorld->SetWorldLocation(TraceHitResult.Location);
	//		cursorToWorld->SetWorldRotation(CursorR);
	//	}
	//}
}
