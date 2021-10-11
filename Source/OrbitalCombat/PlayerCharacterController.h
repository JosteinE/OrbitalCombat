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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMoving{ false };

	UPROPERTY(BlueprintReadOnly)
	bool bIsRunning{ false };

	UPROPERTY(BlueprintReadOnly)
	bool bJumping{ false };

	UPROPERTY(BlueprintReadOnly)
	bool bReadyWeapon{ false };

	UPROPERTY(BlueprintReadOnly)
	bool bLMB{ false };

	bool bUsingController{ false };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Character placement on screen to help controller look direction
	FVector2D characterScreenLocation;
	// Determined by WASD
	FVector directionInput{ 0,0,0 };
	// Determined by the right thumb stick
	FVector2D controllerLookInput{ 0,0 };

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void faceCursorLocation();
	void drawForwardDebugLine();

	// Navigate player using the WASD buttons
	void directionInputToMovement(float deltaTime);
	void jumpInputToMovement();

	/** Input handlers */
	void moveForward(float inputAxis);
	void moveRight(float inputAxis);
	void jump();
	void leftMouseButton();
	void leftMouseButtonReleased();
	void startRunning();
	void stopRunning();
	void fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(FVector projectileLocation, FRotator projectileRotation);
	bool Server_Fire_Validate(FVector projectileLocation, FRotator projectileRotation);
	void Server_Fire_Implementation(FVector projectileLocation, FRotator projectileRotation);

	//Controller specific input
	void controllerLookX(float inputAxis);
	void controllerLookY(float inputAxis);
	void controllerFaceButtonB();
	void controllerRTrigger();
};