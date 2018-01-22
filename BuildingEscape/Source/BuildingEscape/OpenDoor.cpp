// Copyright ShawnCC.Huang 2018

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	Owner = GetOwner();
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	 

	// Create a rotator
	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);

	// Set the door rotation
	Owner->SetActorRotation(NewRotation);
}


void UOpenDoor::CloseDoor()
{
 
	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (ActorThatOpens != NULL  && PressurePlate != NULL)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ActorThatOpens"));
		if (PressurePlate->IsOverlappingActor(ActorThatOpens))
		{
			OpenDoor();
			LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		}
		//Check if it;s time to close the door
		if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
		{
			CloseDoor();

		}


		
	}

 

}

