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
}


void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerPawn( GetWorld(), 0));
	if(bForceOpen)
	{
		OpenDoor(DeltaTime);
	}
	if(PlayerController)
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
				TArray<AActor*> OverlappingActors;
				if(TriggerVol!=nullptr)
				{
					TriggerVol->GetOverlappingActors(OverlappingActors);
					//if overlapping actors is player
					for (AActor* Actor : OverlappingActors)
					{
						if (Cast<AMainPlayerController>(Actor))
						{
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

void UDoor::GetAllValidActors(float DeltaTime)
{
	TArray<AActor*> OverlappingActors;
	if(TriggerVol!=nullptr)
	{
		TriggerVol->GetOverlappingActors(OverlappingActors);
		if (OverlappingActors.Num() > 0)
		{
			OpenDoor(DeltaTime);
		}
		else
		{
			CloseDoor(DeltaTime);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trigger Volume not set for %s"), *GetOwner()->GetName());
	}
}

void UDoor::OpenDoor(float DeltaTime)
{
	float NewZ = StartLocation.Z + TargetHeight;
	Location.Z = FMath::FInterpConstantTo(Location.Z, NewZ, DeltaTime, MoveSpeed);
	GetOwner()->SetActorLocation(Location);
	if(NewZ == Location.Z)
	{
		UE_LOG( LogTemp, Warning, TEXT("Door Opened") );
	}
}

void UDoor::CloseDoor(float DeltaTime)
{
	Location.Z = FMath::FInterpConstantTo(Location.Z, StartLocation.Z, DeltaTime, MoveSpeed);
	GetOwner()->SetActorLocation(Location);
	if(StartLocation.Z == Location.Z)
	{
		UE_LOG( LogTemp, Warning, TEXT("Door Close") );
	}
}

void UDoor::CheckDoorInteraction()
{
	if(DoorCanBeInteracted)
	{
		bForceOpen = true;
	}
}