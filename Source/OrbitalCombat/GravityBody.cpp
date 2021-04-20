// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityBody.h"
#include "EngineUtils.h" // For the iterator to assign planet in begin play
#include "Misc/MessageDialog.h" // For displaying the error message before stopping the game
#include "PlayerCharacterController.h"

class UGravityAttractor;

UGravityBody::UGravityBody()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

UGravityBody::~UGravityBody()
{
}

// Called when the game starts
void UGravityBody::BeginPlay()
{
	Super::BeginPlay();

// Find the main planet actor and assign it to the gravityBody component
	if (planet == nullptr)
	{
		for (TActorIterator<AActor> AActorItr(GetWorld()); AActorItr; ++AActorItr)
		{
			AActor * planetCheck = *AActorItr;
			if (planetCheck->GetActorLabel() == "MainPlanet")
			{
				setPlanetToOrbit(planetCheck);
				break;
			}
		}
	}

	if (planet != nullptr)
	{
		planetAttractor = planet->FindComponentByClass<UGravityAttractor>();
	}
	else
	{
		const FText Message = FText::FromString("Could not find a planet for the GravityBody component.");
		FMessageDialog::Open(EAppMsgType::Ok, FText{ Message }); // Show message
		//GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
	}
}


// Called every frame
void UGravityBody::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	//if (planet != nullptr) // ew gross constant check
	//{
	//	rotateMeshToSurface();
	//	pullMeshToSurface(DeltaTime);
	//}
	//else
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Stopping play as no planet were assigned to the GravityBody component"));
	//}
}

void UGravityBody::setPlanetToOrbit(AActor * inPlanet)
{
	if (inPlanet)
		planet = inPlanet;
	else
		UE_LOG(LogTemp, Warning, TEXT("Attempted to assign a nullptr as planet in a GravityBody component."));
}

void UGravityBody::rotateToSurface()
{
	planetAttractor->RotateToSurface(GetOwner());
}

void UGravityBody::pullToSurface(float deltaTime, bool * bGrounded)
{
	planetAttractor->Attract(GetOwner(), deltaTime, bGrounded);
}

void UGravityBody::rotateMeshToSurface()
{
	planetAttractor->RotateMeshToSurface(GetOwner()->FindComponentByClass<UStaticMeshComponent>());
}

void UGravityBody::pullMeshToSurface(float deltaTime, bool * bGrounded)
{
	planetAttractor->AttractMesh(GetOwner()->FindComponentByClass<UStaticMeshComponent>(), deltaTime, bGrounded);
}

