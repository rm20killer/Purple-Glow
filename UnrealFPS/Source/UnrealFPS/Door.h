// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "InputActionValue.h"
#include "EnhancedInputLibrary.h"
#include "Door.generated.h"

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class UNREALFPS_API UDoor : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputMappingContext* DefaultMappingContext;
	
	// Called every frame
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void GetAllValidActors(float DeltaTime);
	
	UPROPERTY(EditAnywhere, Category = "Door Interaction")
	int TargetNeed = 0;
	UPROPERTY(EditAnywhere, Category = "Door Interaction")
	bool bUsePlayerInRange = false;
	UPROPERTY(EditAnywhere, Category = "Door Interaction")
	int32 KeyNeeded;
	UPROPERTY(EditAnywhere, Category = "Door Interaction")
	bool bUsePlayerButtonPress = false;
	
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void CheckDoorInteraction();

private:
	UPROPERTY(EditAnywhere, Category = "Door Interaction")
	bool bForceOpen = false;
	bool DoorCanBeInteracted=false;
	UPROPERTY(EditAnywhere, Category = "Door")
	float TargetHeight = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
	float MoveSpeed = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
	ATriggerVolume* TriggerVol;

	//inputs
	FVector Location;
	FVector StartLocation;
};