// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECTION_03_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	UPROPERTY(EditAnywhere)
	float InteractDistance = 100.0f;

	bool IsHoldingObject = false;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputControl = nullptr;

	// Check for an input component and set it up
	void SetupInputComponent();

	// Find attached Physics Component
	void FindPhysicsHandleComponent();

	// Set the line for activation of objects
	FVector GetReachLineStart();
	FVector GetReachLineEnd();

	void Interact();
	void Release();

	const FHitResult GetFirstPhysicsBodyInReach();
	const FHitResult GetFirstInteractiveObjectInReach();	
};
