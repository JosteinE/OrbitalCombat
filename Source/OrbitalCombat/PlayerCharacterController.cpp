#include "PlayerCharacterController.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"
//#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"

APlayerCharacterController::APlayerCharacterController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Face the character in the direction of the cursors location
	faceCursorLocation();

	if (bIsMoving)
	{
		directionInputToMovement(DeltaTime);
		bIsMoving = false;
	}
	if (bJumping)
		jumpInputToMovement();
	if (bLMB)
		fire();
}

void APlayerCharacterController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &APlayerCharacterController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &APlayerCharacterController::OnSetDestinationReleased);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterController::moveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterController::moveRight);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacterController::jump);
	InputComponent->BindAction("LMB", IE_Pressed, this, &APlayerCharacterController::leftMouseButton);
	InputComponent->BindAction("LMB", IE_Released, this, &APlayerCharacterController::leftMouseButtonReleased);
	InputComponent->BindAction("Running", IE_Pressed, this, &APlayerCharacterController::startRunning);
	InputComponent->BindAction("Running", IE_Released, this, &APlayerCharacterController::stopRunning);
}

void APlayerCharacterController::faceCursorLocation()
{
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


//void APlayerCharacterController::MoveToMouseCursor()
//{
//	// Trace to see what is under the mouse cursor
//	FHitResult Hit;
//	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
//
//	if (Hit.bBlockingHit)
//	{
//		// We hit something, move there
//		SetNewMoveDestination(Hit.ImpactPoint);
//		UE_LOG(LogTemp, Warning, TEXT("Clicked on %s"), *Hit.GetActor()->GetName());
//	}
//}

void APlayerCharacterController::directionInputToMovement(float deltaTime)
{
	Cast<APlayerCharacter>(GetPawn())->GetMovementComponent()->inputToMovement(directionInput, bJumping, deltaTime);
	directionInput = FVector{ 0,0,0 };
}

void APlayerCharacterController::jumpInputToMovement()
{
	Cast<APlayerCharacter>(GetPawn())->GetMovementComponent()->jump();
	bJumping = false;
}

void APlayerCharacterController::moveByKeyboard(float DeltaTime)
{
	//FVector moveDirection;
	//if (directionInput.X != 0)
	//	moveDirection += GetPawn()->GetActorRightVector() * directionInput.X;

	//if (directionInput.Y != 0)
	//	moveDirection += GetPawn()->GetActorForwardVector() * directionInput.Y;

	//moveDirection.Normalize();

	//UE_LOG(LogTemp, Warning, TEXT("MoveDirection { %f, %f, %f }"), moveDirection.X, moveDirection.Y, moveDirection.Z);
	////UStaticMeshComponent* RootMesh = Cast<UStaticMeshComponent>(GetRootComponent());
	////if (RootMesh != nullptr)
	////	RootMesh->SetPhysicsLinearVelocity(moveDirection * moveSpeed * DeltaTime);
	////else
	////	UE_LOG(LogTemp, Warning, TEXT("Did not find mesh"));
	////GetPawn()->AddActorLocalOffset(moveDirection, true);
	//if(GetPawn()->GetMovementComponent() != nullptr)
	//	GetPawn()->GetMovementComponent()->AddInputVector(moveDirection * moveSpeed * DeltaTime);
	//else
	//	UE_LOG(LogTemp, Warning, TEXT("Could not find momvement component."));
	//directionInput = FVector{ 0,0,0 };
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
	if (projectile)
	{
		//// Set the projectile's initial trajectory.
		//FVector LaunchDirection = projectileSpawnRot.Vector();
		//projectile->FireInDirection(LaunchDirection);
		UE_LOG(LogTemp, Warning, TEXT("Firing!"));
	}
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

//void APlayerCharacterController::SetNewMoveDestination(const FVector DestLocation)
//{
//	APawn* const MyPawn = GetPawn();
//	if (MyPawn)
//	{
//		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
//
//		// We need to issue move command only if far enough in order for walk animation to play correctly
//		if ((Distance > 120.0f))
//		{
//			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
//		}
//	}
//}

//void APlayerCharacterController::OnSetDestinationPressed()
//{
//	// set flag to keep updating destination until released
//	bMoveToMouseCursor = true;
//}
//
//void APlayerCharacterController::OnSetDestinationReleased()
//{
//	// clear flag to indicate we should stop updating the destination
//	bMoveToMouseCursor = false;
//}
