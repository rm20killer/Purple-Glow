// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSystem.h"

#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UScoreSystem::UScoreSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Score = 0;
	HighScore = 0;
	// TimeInMilliseconds = 0;
	TimeInSeconds = 0;
	TotalTimeInSeconds = 0;
	bIsTimerActive = true;
	
	// ...
}


// Called when the game starts
void UScoreSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
/**
 * Get the time in seconds and check what medal the player should have
 * @param DeltaTime 
 * @param TickType 
 * @param ThisTickFunction 
 */
void UScoreSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//increment time
	if(bIsTimerActive)
	{
		TimeInSeconds = GetWorld()->GetTimeSeconds();
		
	}
	if (TimeInSeconds+TotalTimeInSeconds <= PlatinumTime * 60)
	{
		bIsPlatinum = true;
		bIsGold = false;
		bIsSilver = false;
		bIsBronze = false;
	}
	else if (TimeInSeconds+TotalTimeInSeconds <= GoldTime * 60)
	{
		bIsPlatinum = false;
		bIsGold = true;
		bIsSilver = false;
		bIsBronze = false;
	}
	else if (TimeInSeconds+TotalTimeInSeconds <= SilverTime * 60)
	{
		bIsPlatinum = false;
		bIsGold = false;
		bIsSilver = true;
		bIsBronze = false;
	}
	else if (TimeInSeconds+TotalTimeInSeconds <= BrozenTime * 60)
	{
		bIsPlatinum = false;
		bIsGold = false;
		bIsSilver = false;
		bIsBronze = true;
	}
	else
	{
		bIsPlatinum = false;
		bIsGold = false;
		bIsSilver = false;
		bIsBronze = false;
	}
	
	// ...
}

/**
 * add score to the score variable and multiply the score by the medal multiplier
 * @param ScoreToAdd Score that will be added
 */
void UScoreSystem::AddScore(int32 ScoreToAdd)
{
	if(bIsPlatinum)
	{
		ScoreToAdd *= 3.5;
	}
	else if(bIsGold)
	{
		ScoreToAdd *= 3;
	}
	else if(bIsSilver)
	{
		ScoreToAdd *= 2;
	}
	else if(bIsBronze)
	{
		ScoreToAdd *= 1.5;
	}
	Score += ScoreToAdd;
}

/**
 * check if the score is higher than the highscore and if it is set the highscore to the score
 */
void UScoreSystem::SaveScore()
{
	//check if score is higher than highscore
	if(Score > HighScore)
	{
		HighScore = Score;
	}
	//save score
	UFunction* Function = GetOwner()->FindFunction(TEXT("Save"));
	if(Function)
	{
		GetOwner()->ProcessEvent(Function, nullptr);
	}
}

/**
 * return the current score used for the UI
 * @return current score
 */
int32 UScoreSystem::GetScore()
{
	return Score;
}

/**
 * 
 * @return current HighScore
 */
int32 UScoreSystem::GetHighScore()
{
	return HighScore;
}

/**
 * 
 * @return current time in seconds
 */
int64 UScoreSystem::GetTimeInSecound()
{
	return TimeInSeconds+TotalTimeInSeconds;
}


/**
 * finds the minutes and seconds and returns them as a string with the format MM:SS
 * @return current time on the format MM:SS
 */
FString UScoreSystem::GetTimerString()
{
	float total = TimeInSeconds+TotalTimeInSeconds;
	int32 Minutes = FMath::FloorToInt(total / 60);
	int32 Seconds = FMath::FloorToInt(total - (Minutes * 60));
	FString TimerString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	return TimerString;
}

