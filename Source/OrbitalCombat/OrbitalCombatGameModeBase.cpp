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
	planet->SetReplicates(true);
	planet->bNetLoadOnClient = true;

	// Spawn primary player
	APlayerCharacter* mainPlayer = addPlayer();
	Controller = GetWorld()->GetFirstPlayerController();
	mainPlayer->GetGravityBody()->setPlanetToOrbit(planet, planet->getGravityAttractor());
	mainPlayer->GetMovementComponent()->SetComponentTickEnabled(true);
	Controller->Possess(mainPlayer);

	if(mainPlayer->GetGravityBody()->planet)
		UE_LOG(LogTemp, Warning, TEXT("HOST was assigned the planet"));

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
	if (HasActorBegunPlay())
	{
		APlayerCharacter* newPlayerCharacter = addPlayer();
		NewPlayer->Possess(newPlayerCharacter);
		AssignPlanet(NewPlayer);
		newPlayerCharacter->GetMovementComponent()->SetComponentTickEnabled(true);
	}
}

void AOrbitalCombatGameModeBase::AssignPlanet(APlayerController * inPlayer)
{
	Client_AssignPlanet(inPlayer, planet);
	if (Cast<APlayerCharacter>(inPlayer->GetPawn())->GetGravityBody()->planet)
	{
		UE_LOG(LogTemp, Warning, TEXT("GUEST player was assigned the planet"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("GUEST player FAILED to assign the planet"));
}

void AOrbitalCombatGameModeBase::Client_AssignPlanet_Implementation(APlayerController * inPlayer, APlanet * inPlanet)
{
	Cast<APlayerCharacter>(inPlayer->GetPawn())->GetGravityBody()->setPlanetToOrbit(inPlanet, inPlanet->getGravityAttractor());
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
		return newPlayer;
}
