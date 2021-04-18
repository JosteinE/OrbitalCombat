#include "PlayerCharacterController.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"
//#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacterController::APlayerCharacterController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Face the character in the direction of the cursors location
	//faceCursorLocation();

	if (directionInput.X != 0 || directionInput.Y != 0 || directionInput.Z != 0)
		directionInputToMovement(DeltaTime);
	if (bJumping)
		jumpInputToMovement();

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
}

void APlayerCharacterController::faceCursorLocation()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit);

	if (Hit.ImpactPoint.X != lastMouseLoc.X || Hit.ImpactPoint.Y != lastMouseLoc.Y || Hit.ImpactPoint.Z != lastMouseLoc.Z)
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), Hit.ImpactPoint);
		NewRotation.Pitch = 0;
		NewRotation.Roll = 0;
		Cast<APlayerCharacter>(GetPawn())->GetMeshComponent()->SetRelativeRotation(NewRotation);
		UE_LOG(LogTemp, Warning, TEXT("cursorLocation: %f, %f, %f"), Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z);
		//UE_LOG(LogTemp, Warning, TEXT("cursor collided with: %s"), *Hit.GetActor()->GetActorLabel());
		lastMouseLoc = FVector{ Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z };
	}
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

void APlayerCharacterController::moveForward(float inputAxis)
{
	directionInput.Y = inputAxis;
}

void APlayerCharacterController::moveRight(float inputAxis)
{
	directionInput.X = inputAxis;
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
