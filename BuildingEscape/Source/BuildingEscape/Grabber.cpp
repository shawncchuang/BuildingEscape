// Copyright ShawnCC.Huang 2018

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

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
	FindPhysicsComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsComponent()
{

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		// Physics handle is found
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component."), *GetOwner()->GetName())
	}
}

/// Look for attahced Input component (only appears ar run time)
void UGrabber::SetupInputComponent()
{
	InputCom = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputCom)
	{
		InputCom->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputCom->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component."), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	/// Line Trace and see if we reach any actors with physics body collision channely actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // Return PrimitiveComponent
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		/// If we hit somthing then attach a physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation());
	}
}
void UGrabber::Release()
{
	if (!PhysicsHandle)
	{
		return;
	}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle)
	{
		return;
	}

	// if the pyhsics handle is attached
	if (PhysicsHandle->GetGrabbedComponent())
	{
		// move the object that we're handling
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///  Get player view point this tick

	DrawDebugLine(
		GetWorld(),
		GetReachLineStart(),
		GetReachLineEnd(),
		FColor(255, 0, 0),
		false,
		0.f,
		30.f);

	/// Setup query  parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	///  Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	///  Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return LineTraceEnd;
}