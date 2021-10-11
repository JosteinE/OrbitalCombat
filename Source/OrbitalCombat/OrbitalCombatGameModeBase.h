// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Planet.h"
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitalCombatGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ORBITALCOMBAT_API AOrbitalCombatGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbitalCombatGameModeBase();

private:
	void BeginPlay() override;
	void PostLogin(APlayerController* NewPlayer) override;
	void AssignPlanet(APlayerController* inPlayer);

	UFUNCTION(Client, Reliable)
	void Client_AssignPlanet(APlayerController* inPlayer, APlanet* inPlanet);
	void Client_AssignPlanet_Implementation(APlayerController* inPlayer, APlanet* inPlanet);

	APlanet* planet;
	APlayerController* Controller;

public:
	void EndGame();
	APlayerCharacter* addPlayer();

	bool bMouseInUse = false;
	FVector playerSpawnOffset{0.f, 0.f, 600.f};
};
