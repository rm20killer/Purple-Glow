// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Player/MainPlayerController.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UDoor::UDoor()	
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
	//set default values
	TargetHeight = 200.0f;
	MoveSpeed = 20.0f;
	KeyNeeded = 1;
}

// Called when the game starts or when spawned
void UDoor::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetOwner()->GetActorLocation();
	Location = StartLocation;
}


/**
 * Called every frame
 * Check what requirements are needed to open the door
 * Compare the requirements to the player and open the door if the requirements are met
 * else close the door
 * @param DeltaTime 
 * @param TickType 
 * @param ThisTickFunction 
 */
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerPawn( GetWorld(), 0));
	if(bForceOpen)
	{
		OpenDoor(DeltaTime);
	}
	else if(PlayerController)
	{
		if(TargetNeed <= PlayerController->TargetsHit && PlayerController->Keys.Contains(KeyNeeded))
		{
			if(!bUsePlayerButtonPress && !bUsePlayerInRange)
			{
				OpenDoor(DeltaTime);
			}
			else if(bUsePlayerInRange && !bUsePlayerButtonPress)
			{
				GetAllValidActors(DeltaTime);
			}
			else if(bUsePlayerInRange && bUsePlayerButtonPress)
			{
				//check all overlapping actors
				TArray<AActor*> OverlappingActors;
				if(TriggerVol!=nullptr)
				{
					TriggerVol->GetOverlappingActors(OverlappingActors);
					//if overlapping actors is player
					for (AActor* Actor : OverlappingActors)
					{
						if (Cast<AMainPlayerController>(Actor))
						{
							//add the door to the player controller and set the door to be interacted with
							Cast<AMainPlayerController>(Actor)->DoorArr.Add(this->GetOwner());
							DoorCanBeInteracted = true;
						}
						else
						{
							DoorCanBeInteracted = false;
						}
					}
				}
			}
			else
			{
				CloseDoor(DeltaTime);
			}
		}
		else
		{
			CloseDoor(DeltaTime);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error , TEXT("Player Controller not found"))
	}
}

/**
 * Check is in the trigger volume and if so open the door
 * and if not close the door
 * Warning: This function if the trigger volume is not set
 * @param DeltaTime 
 */
void UDoor::GetAllValidActors(float DeltaTime)
{
	bool bIsPlayerInRange = false;
	if(TriggerVol!=nullptr)
	{
		TArray<AActor*> OverlappingActors;
		TriggerVol->GetOverlappingActors(OverlappingActors);
		// UE_LOG(LogTemp, Warning, TEXT("Overlapping Actors: %d"), OverlappingActors.Num());
		for (AActor* Actor : OverlappingActors)
		{
			if (Cast<AMainPlayerController>(Actor))
			{
				bIsPlayerInRange = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trigger Volume not set for %s"), *GetOwner()->GetName());
	}


	if(bIsPlayerInRange)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	
	}
}

/**
 * Open the door by moving it up
 * @param DeltaTime 
 */
void UDoor::OpenDoor(float DeltaTime)
{
	//get the current location of the door
	float NewZ = StartLocation.Z + TargetHeight;
	//interpolate the location of the door
	Location.Z = FMath::FInterpConstantTo(Location.Z, NewZ, DeltaTime, MoveSpeed);
	//set the location of the door
	GetOwner()->SetActorLocation(Location);
	//if the door is at the target height log that the door is open
	if(NewZ == Location.Z)
	{
		// UE_LOG( LogTemp, Warning, TEXT("Door Opened") );
	}
}

/**
 * Close the door by moving it down to the start location
 * @param DeltaTime 
 */
void UDoor::CloseDoor(float DeltaTime)
{
	//interpolate the location of the door to the start location
	Location.Z = FMath::FInterpConstantTo(Location.Z, StartLocation.Z, DeltaTime, MoveSpeed);
	//set the location of the door
	GetOwner()->SetActorLocation(Location);
	if(StartLocation.Z == Location.Z)
	{
		// UE_LOG( LogTemp, Warning, TEXT("Door Close") );
	}
}

/**
 * If the door can be interacted with force the door to open
 * Used by the player to open the door
 */
void UDoor::CheckDoorInteraction()
{
	if(DoorCanBeInteracted)
	{
		bForceOpen = true;
	}
}