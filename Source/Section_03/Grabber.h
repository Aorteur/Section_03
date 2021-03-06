// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECTION_03_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 150.0f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputControl = nullptr;

	// Ray-cast and grab the moveble object
	void Grab();

	void Release();

	// Find attached Physics Component
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	const FHitResult GetFirstPhysicsBodyInReach();
	const FHitResult GetFirstInteractiveObjectInReach();

	FVector GetReachLineStart();
	FVector GetReachLineEnd();
};
