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

	if ((SwitchPowered) && (SwitchDelayTime >= 0.0f))
	{
		if (GetWorld()->GetTimeSeconds() - LastTimeSwitchSet > SwitchDelayTime)
		{
			SwitchOff();
		}
	}
}

void USwitchComponent::SwitchOn()
{
	if (SwitchDelayTime >= 0.0f)
	{
		LastTimeSwitchSet = GetWorld()->GetTimeSeconds();		
	}
	if (TimesSwitched != MaxTimesSwitched)
	{
		if (MaxTimesSwitched > 0) 
		{
			TimesSwitched++;
		}
		SwitchPowered = true;
	}


}

void USwitchComponent::SwitchOff()
{
	if (TimesSwitched != MaxTimesSwitched)
	{
		SwitchPowered = false;
	}
}

void USwitchComponent::SwitchToggle()
{
	if (SwitchPowered) 
	{
		SwitchOff();
	}
	else
	{
		SwitchOn();
	}
}
