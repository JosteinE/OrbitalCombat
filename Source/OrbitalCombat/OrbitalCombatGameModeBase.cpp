// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbitalCombatGameModeBase.h"
#include "Kismet/GameplayStatics.h" // Open level
#include "Engine/Engine.h" // for GEngine warnings
#include "Engine/LocalPlayer.h"

AOrbitalCombatGameModeBase::AOrbitalCombatGameModeBase()
{
	//DefaultPawnClass = APlayerCharacter::StaticClass();

	//If you need multiple classes, look into overriding the GetDefaultPawnClassForController() function in C++ / Blueprint.
	//static ConstructorHelpers::FObjectFinder<UClass> BP_DefaultPawnClass(TEXT("/Game/Blueprints/PlayerCharacter_BP"));
	//DefaultPawnClass = (BP_DefaultPawnClass.Object != nullptr) ? BP_DefaultPawnClass.Object : APlayerCharacter::StaticClass();
	PlayerControllerClass = APlayerCharacterController::StaticClass();
}

void AOrbitalCombatGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//FInputModeGameOnly InputMode;
	//Controller->SetInputMode(InputMode);

	//UGameplayStatics::OpenLevel(this, "MainMap", true);

	// Generate the planet
	//planet = NewObject<APlanet>(APlanet::StaticClass(), TEXT("Planet"));
	FActorSpawnParameters planetSpawnParams;
	planetSpawnParams.Owner = this;
	planet = GetWorld()->SpawnActor<APlanet>(APlanet::StaticClass(), FVector{ 0.f, 0.f, 0.f }, FRotator{ 0.f, 0.f, 0.f }, planetSpawnParams);
	planet->SetActorScale3D(FVector{ 10.f, 10.f, 10.f });

	// Spawn primary player
	APlayerCharacter* mainPlayer = addPlayer();
	Controller = GetWorld()->GetFirstPlayerController();
	Controller->Possess(mainPlayer);

	// Spawn local players
	if (GetNumPlayers() > 1)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController != Controller)
			{
				APlayerCharacter* localPlayer = addPlayer();
				Cast<APlayerCharacterController>(PlayerController)->bUsingController = true;
				PlayerController->Possess(localPlayer);
			}
		}
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
