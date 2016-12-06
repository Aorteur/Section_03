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

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Component on %s."), *(GetOwner()->GetName()));
	}
}

// Look for attached Input Component and bind inputs
void UGrabber::SetupInputComponent()
{
	InputControl = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputControl)
	{ 
		InputControl->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputControl->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component on %s."), *(GetOwner()->GetName()));
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

void UGrabber::Grab()
{
	// Line trace and see if we reach any actors with collision body set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		// TODO If we hit object, attach a physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

// Release the held component
void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

// Find the first physics body in reach
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

// Find the start of the Ray Trace Reach Line
FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return (PlayerViewPointLocation);
}

// Find the end of the Ray Trace Reach Line
FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return (PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach);
}