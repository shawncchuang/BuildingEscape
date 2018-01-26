// Copyright ShawnCC.Huang 2018

#pragma once

#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenRequest, float, OpenDoorTime);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

  public:
	// Sets default values for this component's properties
	UOpenDoor();
	float OpenDoorTime = 1.f;

  protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	
	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;

  private:
	UPROPERTY(EditAnyWhere)
	float OpenAngle = -90.0f;

	UPROPERTY(EditAnyWhere)
	ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY(EditAnyWhere)
	float DoorCloseDelay = 1.f;

	float LastDoorOpenTime;

	AActor *Owner = nullptr; // The  owning door

	// Returns total mass in kg
	float GetTotalMassOfActorOnPlate();
};
