// Fill out your copyright notice in the Description page of Project Settings.

#include "Section_03.h"
#include "OpenDoor.h"
#include "SwitchComponent.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("No Pressure Plate set for door"));
	}
	
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume every frame
	if ((GetTotalMassOfActorsOnPlate() > TriggerMass) || (GetSwitchStatus()))
	{
		OnOpen.Broadcast();
	} 
	else 
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;

	if (PressurePlate)
	{
		// Find all the overlapping actors
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		for (const auto& OverlapActor : OverlappingActors)
		{
			TotalMass += OverlapActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

			UE_LOG(LogTemp, Warning, TEXT("Overlap %s"), *((OverlapActor)->GetName()));
		}
	}
	return TotalMass;
}

bool UOpenDoor::GetSwitchStatus()
{
	if (LinkedSwitch)
	{
		auto MySwitchComponent = LinkedSwitch->FindComponentByClass<USwitchComponent>();
		if (MySwitchComponent) {
			return (MySwitchComponent->IsSwitchPowered());
		}
	}
	return false;
}

