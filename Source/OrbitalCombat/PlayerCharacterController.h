#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Projectile.h"
#include "PlayerCharacterController.generated.h"

UCLASS()
class APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

	UPROPERTY(BlueprintReadOnly)
	bool bIsMoving{ false };

	UPROPERTY(BlueprintReadOnly)
	bool bIsRunning{ false };

	UPROPERTY(BlueprintReadOnly)
	bool bJumping{ false };

	UPROPERTY(BlueprintReadOnly)
	bool bLMB{ false };

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	//uint32 bMoveToMouseCursor : 1;

	// Determined by WASD
	FVector directionInput{ 0,0,0 };
	FVector controllerLookInput{ 0,0,0 };
	bool bControllerLookRotated{ false };

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void faceCursorLocation();
	void faceControllerRotation();
	void drawForwardDebugLine();

	/** Navigate player to the current mouse cursor location. */
	//void MoveToMouseCursor();

	// Navigate player using the WASD buttons
	void directionInputToMovement(float deltaTime);
	void jumpInputToMovement();
	void moveByKeyboard(float DeltaTime);

	/** Navigate player to the given world location. */
	//void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	//void OnSetDestinationPressed();
	//void OnSetDestinationReleased();

	/** Input handlers for WASD */
	void moveForward(float inputAxis);
	void moveRight(float inputAxis);
	void controllerLookX(float inputAxis);
	void controllerLookY(float inputAxis);
	void jump();
	void leftMouseButton();
	void leftMouseButtonReleased();
	void startRunning();
	void stopRunning();
	void fire();
};