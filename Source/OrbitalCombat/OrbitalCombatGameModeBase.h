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

	APlanet* planet;
	APlayerController* Controller;

public:
	void EndGame();
	APlayerCharacter* addPlayer(APlayerCharacter* player = nullptr, bool bIsLocal = true);

	int numPlayers = 0;
	int numLocalPlayers = 0;
	bool bMouseInUse = false;
	FVector playerSpawnOffset{0.f, 0.f, 600.f};
};
