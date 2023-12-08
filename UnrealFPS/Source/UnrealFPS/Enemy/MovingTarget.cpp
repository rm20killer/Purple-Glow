// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingTarget.h"

#include "../Player/ScoreSystem.h"
#include "UnrealFPS/Player/MainPlayerController.h"

// Sets default values
AMovingTarget::AMovingTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
		RootComponent = CollisionComponent;
	}

	Endpoint = FVector(0.0f, 0.0f, 0.0f);
	Startingpoint = FVector(0.0f, 100.0f, 0.0f);
}

// Called when the game starts or when spawned
void AMovingTarget::BeginPlay()
{
	Super::BeginPlay();
	Startingpoint = GetRootComponent()->GetComponentLocation();
	Endpoint = Startingpoint + Endpoint;
}

// Called every frame
void AMovingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Calculate the distance to travel
	FVector TargetLocation = (MovingBackToStart ? Startingpoint : Endpoint);

	// Interpolate the object's position towards the target
	float NewX = FMath::FInterpConstantTo(GetRootComponent()->GetRelativeLocation().X, TargetLocation.X, DeltaTime, MovingTargetSpeed);
	float NewY = FMath::FInterpConstantTo(GetRootComponent()->GetRelativeLocation().Y, TargetLocation.Y, DeltaTime, MovingTargetSpeed);
	float NewZ = FMath::FInterpConstantTo(GetRootComponent()->GetRelativeLocation().Z, TargetLocation.Z, DeltaTime, MovingTargetSpeed);
	
	FVector InterpLocation = FVector(NewX, NewY, NewZ);
	// Set the object's position to the interpolated location
	GetRootComponent()->SetRelativeLocation(InterpLocation);

	// Check if the target has been reached
	if (GetRootComponent()->GetRelativeLocation() == TargetLocation)
	{
		// If so, reverse the direction
		MovingBackToStart = !MovingBackToStart;
	}
}

void AMovingTarget::TargetHit()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//if player controller is not null
	if (PlayerController)
	{
		//get player pawn
		APawn* Pawn = PlayerController->GetPawn();
		//if pawn is not null
		if (Pawn)
		{
			//get player pawn's score system
			UScoreSystem* ScoreSystem = Pawn->FindComponentByClass<UScoreSystem>();
			//if score system is not null
			if (ScoreSystem)
			{
				//increment score
				ScoreSystem->AddScore(100);
			}
			AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController);
			if (MainPlayerController)
			{
				MainPlayerController->TargetsHit++;
				UE_LOG(LogTemp, Warning, TEXT("Target hit %d"), MainPlayerController->TargetsHit);
			}
		}
		Destroy();
	}
}
