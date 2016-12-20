// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SwitchComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECTION_03_API USwitchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwitchComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void SwitchOn();
	void SwitchOff();
	void SwitchToggle();
	bool IsSwitchPowered();

	UPROPERTY(EditAnywhere)
	float SwitchDelayTime = -1.0f;
	float LastTimeSwitchSet = 0.0f;

	UPROPERTY(EditAnywhere)
	int32 MaxTimesSwitched = -1;
	int32 TimesSwitched = 0;

private:
	bool SwitchPowered = false;
	
};
