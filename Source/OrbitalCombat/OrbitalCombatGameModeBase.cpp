// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbitalCombatGameModeBase.h"
#include "Kismet/GameplayStatics.h" // Open level
#include "Engine/Engine.h" // for GEngine warnings
#include "Engine/LocalPlayer.h"

AOrbitalCombatGameModeBase::AOrbitalCombatGameModeBase()
{
	PlayerControllerClass = APlayerCharacterController::StaticClass();
}

void AOrbitalCombatGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//FInputModeGameOnly InputMode;
	//Controller->SetInputMode(InputMode);

	//UGameplayStatics::OpenLevel(this, "MainMap", true);

	FActorSpawnParameters planetSpawnParams;
	planetSpawnParams.Owner = this;
	planet = GetWorld()->SpawnActor<APlanet>(APlanet::StaticClass(), FVector{ 0.f, 0.f, 0.f }, FRotator{ 0.f, 0.f, 0.f }, planetSpawnParams);
	planet->SetActorScale3D(FVector{ 10.f, 10.f, 10.f });

	// Spawn primary player
	APlayerCharacter* mainPlayer = addPlayer();
	Controller = GetWorld()->GetFirstPlayerController();
	Controller->Possess(mainPlayer);

	worldGenerated = true;

	// Spawn local players
	//if (GetNumPlayers() > 1)
	//{
	//	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	//	{
	//		APlayerController* PlayerController = Iterator->Get();
	//		if (PlayerController != Controller)
	//		{
	//			APlayerCharacter* localPlayer = addPlayer();
	//			Cast<APlayerCharacterController>(PlayerController)->bUsingController = true;
	//			PlayerController->Possess(localPlayer);
	//		}
	//	}
	//}
}

void AOrbitalCombatGameModeBase::PostLogin(APlayerController * NewPlayer)
{
	if (worldGenerated)
	{
		APlayerCharacter* newPlayerCharacter = addPlayer();
		Cast<APlayerCharacterController>(NewPlayer)->bUsingController = true;
		NewPlayer->Possess(newPlayerCharacter);
	}
}


void AOrbitalCombatGameModeBase::EndGame()
{
}

APlayerCharacter* AOrbitalCombatGameModeBase::addPlayer()
{
		TSubclassOf<APlayerCharacter> pawnClass = LoadObject<UBlueprint>(NULL, TEXT("/Game/Blueprints/PlayerCharacter_BP"))->GeneratedClass;

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APlayerCharacter* newPlayer = World->SpawnActor<APlayerCharacter>(pawnClass, planet->GetActorLocation() + playerSpawnOffset, FRotator{0,0,0}, SpawnParams);

		newPlayer->GetGravityBody()->setPlanetToOrbit(planet, planet->getGravityAttractor());
		newPlayer->GetMovementComponent()->SetComponentTickEnabled(true);

		return newPlayer;
}
