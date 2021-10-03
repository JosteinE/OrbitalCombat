// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbitalCombatGameModeBase.h"
#include "Kismet/GameplayStatics.h" // Open level
#include "Engine/Engine.h" // for GEngine warnings
#include "UObject/ConstructorHelpers.h"

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

	// Spawn player
	APlayerCharacter* mainPlayer = addPlayer(NULL, true);
	Controller = GetWorld()->GetFirstPlayerController();
	Controller->Possess(mainPlayer);
}


void AOrbitalCombatGameModeBase::EndGame()
{
}

APlayerCharacter* AOrbitalCombatGameModeBase::addPlayer(APlayerCharacter* player, bool bIsLocal)
{
	if (bIsLocal)
		numLocalPlayers++;
	numPlayers++;

	if (!player)
	{
		TSubclassOf<APlayerCharacter> pawnClass = LoadObject<UBlueprint>(NULL, TEXT("/Game/Blueprints/PlayerCharacter_BP"))->GeneratedClass;

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APlayerCharacter* newPlayer = World->SpawnActor<APlayerCharacter>(pawnClass, planet->GetActorLocation() + playerSpawnOffset, FRotator{0,0,0}, SpawnParams);

		newPlayer->GetGravityBody()->setPlanetToOrbit(planet, planet->getGravityAttractor());
		newPlayer->GetMovementComponent()->SetComponentTickEnabled(true);

		UE_LOG(LogTemp, Warning, TEXT("Player: %i generated"), numPlayers);
		return newPlayer;
	}

	//if (!player)
	//	player = NewObject<APlayerCharacter>(APlayerCharacter::StaticClass(), TEXT("playerCharacter"));

	//player->GetGravityBody()->setPlanetToOrbit(planet, planet->getGravityAttractor());

	return nullptr;
}
