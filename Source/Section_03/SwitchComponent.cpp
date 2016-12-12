// Fill out your copyright notice in the Description page of Project Settings.

#include "Section_03.h"
#include "SwitchComponent.h"


// Sets default values for this component's properties
USwitchComponent::USwitchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

bool USwitchComponent::IsSwitchPowered() { return SwitchPowered; }

// Called when the game starts
void USwitchComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USwitchComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void USwitchComponent::SwitchOn()
{
	SwitchPowered = true;
}

void USwitchComponent::SwitchOff()
{
	SwitchPowered = false;
}

void USwitchComponent::SwitchToggle()
{
	SwitchPowered = !SwitchPowered;
}
