// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingTarget.h"

#include "Player/ScoreSystem.h"

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
	
}

// Called every frame
void AMovingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Currentloctaion;
	if (MovingBackToStart)
	{
		GetRootComponent()->SetRelativeLocation(Endpoint);
		if (GetRootComponent()->GetRelativeLocation() == Endpoint)
		{
			MovingBackToStart = true;
		}
	}
	else
		{
		GetRootComponent()->SetRelativeLocation(Startingpoint);
		if (GetRootComponent()->GetRelativeLocation() == Startingpoint)
		{
			MovingBackToStart = false;
		}
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
		}
		Destroy();
	}
}

