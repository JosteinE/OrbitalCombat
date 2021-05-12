#include "PlayerCharacterController.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"
//#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

APlayerCharacterController::APlayerCharacterController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	//Assign these variables the size of the screen to help guide the controller look rotator in the faceControllerRotation function.
	GetViewportSize(screenX, screenY);
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Face the character in the direction of the cursors location
	faceCursorLocation();

	if(directionInput.X == 0 && directionInput.Y == 0 && bIsMoving)
		bIsMoving = false;
	else if (bIsMoving)
	{
		directionInputToMovement(DeltaTime);
		directionInput = FVector{ 0,0,0 };
	}

	if (bJumping)
		jumpInputToMovement();
	if (bLMB && bIsRunning)
		fire();
}

void APlayerCharacterController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterController::moveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterController::moveRight);

	InputComponent->BindAxis("ControllerLookY", this, &APlayerCharacterController::controllerLookY);
	InputComponent->BindAxis("ControllerLookX", this, &APlayerCharacterController::controllerLookX);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacterController::jump);
	InputComponent->BindAction("LMB", IE_Pressed, this, &APlayerCharacterController::leftMouseButton);
	InputComponent->BindAction("LMB", IE_Released, this, &APlayerCharacterController::leftMouseButtonReleased);
	InputComponent->BindAction("Running", IE_Pressed, this, &APlayerCharacterController::startRunning);
	InputComponent->BindAction("Running", IE_Released, this, &APlayerCharacterController::stopRunning);
}

void APlayerCharacterController::faceCursorLocation()
{
	if(bUsingController)
		SetMouseLocation(screenX * 0.5f + controllerLookInput.X * 100.f, screenY * 0.5f + controllerLookInput.Y * 100.f);
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit);
	Cast<APlayerCharacter>(GetPawn())->GetMeshComponent()->SetWorldRotation(FRotationMatrix::MakeFromXZ(Hit.ImpactPoint - GetPawn()->GetActorLocation(), GetPawn()->GetActorUpVector()).ToQuat());
}

void APlayerCharacterController::drawForwardDebugLine()
{
	FVector forwardVector = Cast<APlayerCharacter>(GetPawn())->GetMeshComponent()->GetForwardVector();
	forwardVector.Normalize();
	forwardVector *= 500.f;
	DrawDebugLine(GetWorld(), GetPawn()->GetActorLocation(), GetPawn()->GetActorLocation() + forwardVector, FColor::Red, true, 5.f);
}

void APlayerCharacterController::directionInputToMovement(float deltaTime)
{
	Cast<APlayerCharacter>(GetPawn())->GetMovementComponent()->inputToMovement(directionInput, bJumping, deltaTime);
}

void APlayerCharacterController::jumpInputToMovement()
{
	Cast<APlayerCharacter>(GetPawn())->GetMovementComponent()->jump();
	bJumping = false;
}

void APlayerCharacterController::jump()
{
	bJumping = true;
	UE_LOG(LogTemp, Warning, TEXT("JumpPressed"));
}

void APlayerCharacterController::leftMouseButton()
{
	bLMB = true;
}

void APlayerCharacterController::leftMouseButtonReleased()
{
	fire();
	bLMB = false;
}

void APlayerCharacterController::startRunning()
{
	bIsRunning = true;
}

void APlayerCharacterController::stopRunning()
{
	bIsRunning = false;
}

void APlayerCharacterController::fire()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	FVector projectileSpawnLoc = Cast<APlayerCharacter>(GetPawn())->GetMeshComponent()->GetForwardVector();
	projectileSpawnLoc.Normalize();
	projectileSpawnLoc *= 2;
	projectileSpawnLoc += GetPawn()->GetActorLocation();

	FRotator projectileSpawnRot = Cast<APlayerCharacter>(GetPawn())->GetMeshComponent()->GetComponentRotation();

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), projectileSpawnLoc, projectileSpawnRot, spawnParams);

	projectile->GetGravityBody()->setPlanetToOrbit(Cast<APlayerCharacter>(GetPawn())->GetGravityBody()->planet);
}

void APlayerCharacterController::moveForward(float inputAxis)
{
	directionInput.Y = inputAxis;
	bIsMoving = true;
}

void APlayerCharacterController::moveRight(float inputAxis)
{
	directionInput.X = inputAxis;
	bIsMoving = true;
}

void APlayerCharacterController::controllerLookX(float inputAxis)
{
	controllerLookInput.X = inputAxis;
	bUsingController = true; // NEED A BETTER SOLUTION TO THIS
}

void APlayerCharacterController::controllerLookY(float inputAxis)
{
	controllerLookInput.Y = inputAxis;
	bUsingController = true; // NEED A BETTER SOLUTION TO THIS
}