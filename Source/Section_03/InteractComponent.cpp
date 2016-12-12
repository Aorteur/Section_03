// Fill out your copyright notice in the Description page of Project Settings.

#include "Section_03.h"
#include "InteractComponent.h"

#define OUT

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}


// Called every frame
void UInteractComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

// Look for attached Physics Handle
void UInteractComponent::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Component on %s."), *(GetOwner()->GetName()));
	}
}

// Look for attached Input Component and bind inputs
void UInteractComponent::SetupInputComponent()
{
	InputControl = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputControl)
	{
		InputControl->BindAction("Use", EInputEvent::IE_Pressed, this, &UInteractComponent::Interact);
		InputControl->BindAction("Use", EInputEvent::IE_Released, this, &UInteractComponent::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component on %s."), *(GetOwner()->GetName()));
	}
}

// Find the start of the Ray Trace Reach Line
FVector UInteractComponent::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return (PlayerViewPointLocation);
}

// Find the end of the Ray Trace Reach Line
FVector UInteractComponent::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return (PlayerViewPointLocation + PlayerViewPointRotation.Vector() * InteractDistance);
}

void UInteractComponent::Interact()
{
	FHitResult HitResult = GetFirstInteractiveObjectInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		if (ComponentToGrab->GetCollisionObjectType() == ECC_PhysicsBody)
		{
			UE_LOG(LogTemp, Error, TEXT("Attempting Grab"));

			IsHoldingObject = true;
			PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
		}
		else
		{
			// Activate the object
		}

	}
}

// Release the held component
void UInteractComponent::Release()
{
	if (!PhysicsHandle) { return; }

	if (PhysicsHandle->GrabbedComponent)
	{
		IsHoldingObject = false;
		PhysicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Error, TEXT("Attempting Release"));

	}
}

// Find the first physics body in reach
const FHitResult UInteractComponent::GetFirstPhysicsBodyInReach()
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
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Object: %s"), *(ActorHit->GetName()));

	}
	return HitResult;
}

const FHitResult UInteractComponent::GetFirstInteractiveObjectInReach()
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	UE_LOG(LogTemp, Error, TEXT("Attempting Object Trace"));
	GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		ECollisionChannel::ECC_Visibility);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Object: %s"), *(ActorHit->GetName()));

	}


	/* FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
	OUT HitResult,
	GetReachLineStart(),
	GetReachLineEnd(),
	FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic
	);
	*/
	return HitResult;
}
