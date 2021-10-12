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
	bAllowTickBeforeBeginPlay = false;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Face the character in the direction of the cursors location
	faceCursorLocation();

	if (directionInput.X == 0 && directionInput.Y == 0 && bIsMoving)
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
	InputComponent->BindAction("ControllerFaceButtonB", IE_Pressed, this, &APlayerCharacterController::controllerFaceButtonB);
	InputComponent->BindAction("ControllerRTrigger", IE_Pressed, this, &APlayerCharacterController::controllerRTrigger);
}

void APlayerCharacterController::faceCursorLocation()
{
	FHitResult Hit;
	UPrimitiveComponent* cursorCollider = Cast<APlayerCharacter>(GetPawn())->GetCursorPlaneMeshComponent();
	FVector2D screenLocation;
	FVector cursorWorldLocation, cursorWorldDirection;

	if (bUsingController)
	{
		if (controllerLookInput.Size() > 0)
		{
			bReadyWeapon = true;
			screenLocation = characterScreenLocation + controllerLookInput * 100.f;
		}
		else
		{
			bReadyWeapon = false;
			return; // Prevents the character from resetting their rotation
		}
	}
	else
		GetMousePosition(screenLocation.X, screenLocation.Y);

	// Convert screen position to world position
	DeprojectScreenPositionToWorld(screenLocation.X, screenLocation.Y, cursorWorldLocation, cursorWorldDirection);

	// Making sure we're only colliding with the players belonging cursor collider
	if(cursorCollider->LineTraceComponent(Hit, cursorWorldLocation, cursorWorldLocation + cursorWorldDirection * 10000.f, FCollisionQueryParams()))
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
	bReadyWeapon = true;
}

void APlayerCharacterController::leftMouseButtonReleased()
{
	bLMB = false;
	bReadyWeapon = false;
	fire();
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
	spawnParams.Owner = Cast<APlayerCharacter>(GetPawn());
	spawnParams.Instigator = GetInstigator();

	FVector projectileSpawnLoc = Cast<APlayerCharacter>(GetPawn())->GetMeshComponent()->GetForwardVector();
	projectileSpawnLoc.Normalize();
	projectileSpawnLoc *= 2;
	projectileSpawnLoc += GetPawn()->GetActorLocation();

	FRotator projectileSpawnRot = Cast<APlayerCharacter>(GetPawn())->GetMeshComponent()->GetComponentRotation();

	if(!GetWorld()->IsServer())
		Server_Fire(projectileSpawnLoc, projectileSpawnRot);
	else
	{
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), projectileSpawnLoc, projectileSpawnRot, spawnParams);
		projectile->setPlanet(Cast<APlayerCharacter>(GetPawn())->GetGravityBody()->planet, Cast<APlayerCharacter>(GetPawn())->GetGravityBody()->planetAttractor);
		projectile->PrimaryActorTick.SetTickFunctionEnable(true);
	}
}

bool APlayerCharacterController::Server_Fire_Validate(FVector projectileLocation, FRotator projectileRotation)
{
	return true;
}

void APlayerCharacterController::Server_Fire_Implementation(FVector projectileLocation, FRotator projectileRotation)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = Cast<APlayerCharacter>(GetPawn());
	spawnParams.Instigator = GetInstigator();
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), projectileLocation, projectileRotation, spawnParams);
	projectile->setPlanet(Cast<APlayerCharacter>(GetPawn())->GetGravityBody()->planet, Cast<APlayerCharacter>(GetPawn())->GetGravityBody()->planetAttractor);
	projectile->PrimaryActorTick.SetTickFunctionEnable(true);
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
}

void APlayerCharacterController::controllerLookY(float inputAxis)
{
	controllerLookInput.Y = inputAxis;
}

void APlayerCharacterController::controllerFaceButtonB()
{
	// Enables character rotation via the right thumb stick
	bUsingController = true;
	
	ProjectWorldLocationToScreen(GetPawn()->GetActorLocation(), characterScreenLocation);
}

void APlayerCharacterController::controllerRTrigger()
{
	if (bReadyWeapon)
	{
		fire();
		bReadyWeapon = false;
	}
}