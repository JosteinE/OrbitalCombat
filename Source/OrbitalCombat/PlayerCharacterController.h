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
	// Determined by WASD
	FVector directionInput{ 0,0,0 };
	// Determined by the right thumb stick
	FVector controllerLookInput{ 0,0,0 };
	bool bControllerLookRotated{ false };

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void faceCursorLocation();
	void faceControllerRotation();
	void drawForwardDebugLine();

	// Navigate player using the WASD buttons
	void directionInputToMovement(float deltaTime);
	void jumpInputToMovement();

	/** Input handlers */
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