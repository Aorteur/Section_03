// Fill out your copyright notice in the Description page of Project Settings.

#include "Section_03.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	UE_LOG(LogTemp, Warning, TEXT("Grabber ready on %s."), *(GetOwner()->GetName()));
	
	// look for attached component

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Component on %s."), *(GetOwner()->GetName()));

	}

	InputControl = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputControl)
	{ 
		// We can has input
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found."));
		// Bind the input

		InputControl->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component on %s."), *(GetOwner()->GetName()));

	}

}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed."));

}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Viewpoint: %s and %s"), 
	//		*PlayerViewPointLocation.ToString(),
	//		*PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation 
			+ PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);

	// Set up query 
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line Trace
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(ActorHit->GetName()));
	}

}

