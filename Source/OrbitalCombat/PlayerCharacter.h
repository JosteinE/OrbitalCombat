// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GravityBody.h"
#include "PlayerCharacterController.h"
#include "OrbitalMovementComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS(Blueprintable)
class APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int playerScore = 0;

	/** Returns playerController subobject **/
	//FORCEINLINE class APlayerCharacterController* GetController() const { return playerController; }
	/** Returns playerCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return playerCameraComponent; }
	/** Returns cameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return cameraBoom; }
	/** Returns playerCapsuleComponent subobject **/
	FORCEINLINE class UCapsuleComponent* GetCapsuleComponent() { return playerCapsuleComponent; }
	/** Returns the movementComponent subobject **/
	FORCEINLINE class UOrbitalMovementComponent* GetMovementComponent() { return movementComponent; }
	/** Returns the meshComponent subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMeshComponent () { return skeletalMeshComponent; }
	/** Returns the gravityBody subobject **/
	FORCEINLINE class UGravityBody* GetGravityBody() { return gravityBody; }
	/** Returns the cursorPlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetCursorPlaneMeshComponent() { return cursorPlaneMesh; }

	/** Returns cursorToWorld subobject **/
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return cursorToWorld; }

private:

	/** Input controller for the player */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = playerController, meta = (AllowPrivateAccess = "true"))
	//class APlayerCharacterController* playerController;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* playerCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* cameraBoom;

	/** The players collision detector. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = collisionComponent, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* playerCapsuleComponent;

	///** A decal that projects to the cursor location. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//	class UDecalComponent* cursorToWorld;

	/** Mesh that represents the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = skeletalMesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* skeletalMeshComponent;

	/** Movement controller for the player */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = movementComponent, meta = (AllowPrivateAccess = "true"))
	class UOrbitalMovementComponent* movementComponent;

	/** Gravity body - Rotates (and pulls) owner towards a target */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = gravityBody, meta = (AllowPrivateAccess = "true"))
	class UGravityBody *gravityBody;

	/** Mesh that will collide with the cursor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = cursorPlane, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* cursorPlaneMesh;
};

