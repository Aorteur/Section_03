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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Component on %s."), *(GetOwner()->GetName()));

	}

}

// Look for attached Input Component and set keys
void UGrabber::SetupInputComponent()
{
	InputControl = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputControl)
	{ 
		// Bind the input
		InputControl->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputControl->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component on %s."), *(GetOwner()->GetName()));
	}
}



void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed."));

	// Line trace and see if we reach any actors with collision body set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		// TODO If we hit object, attach a physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released."));

	// TODO release physics handle
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}

}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation);

		FVector LineTraceEnd = PlayerViewPointLocation
			+ PlayerViewPointRotation.Vector() * Reach;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation
		+ PlayerViewPointRotation.Vector() * Reach;

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
	return HitResult;
}
