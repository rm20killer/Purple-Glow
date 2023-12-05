// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSystem.h"

// Sets default values for this component's properties
UScoreSystem::UScoreSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Score = 0;
	HighScore = 0;
	TimeInMilliseconds = 0;
	TimeInSeconds = 0;
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
void UScoreSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//increment time
	if(bIsTimerActive)
	{
		TimeInMilliseconds += DeltaTime * 10;
	}
	if (TimeInMilliseconds <= PlatinumTime * 1000)
	{
		bIsPlatinum = true;
		bIsGold = false;
		bIsSilver = false;
		bIsBronze = false;
	}
	else if (TimeInMilliseconds <= GoldTime * 1000)
	{
		bIsPlatinum = false;
		bIsGold = true;
		bIsSilver = false;
		bIsBronze = false;
	}
	else if (TimeInMilliseconds <= SilverTime * 1000)
	{
		bIsPlatinum = false;
		bIsGold = false;
		bIsSilver = true;
		bIsBronze = false;
	}
	else if (TimeInMilliseconds <= BrozenTime * 1000)
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

void UScoreSystem::SaveScore()
{
	//check if score is higher than highscore
	if(Score > HighScore)
	{
		HighScore = Score;
	}
}

int32 UScoreSystem::GetScore()
{
	return Score;
}

int32 UScoreSystem::GetHighScore()
{
	return HighScore;
}

int64 UScoreSystem::GetTimeInMilliseconds()
{
	return TimeInMilliseconds;
}

FString UScoreSystem::GetTimerString()
{
	
	int32 Minutes = TimeInMilliseconds / 60;
	int32 Seconds = TimeInMilliseconds % 60;
	int32 Milliseconds = TimeInMilliseconds % 1000;
	FString TimerString = FString::Printf(TEXT("%02d:%02d:%03d"), Minutes, Seconds, Milliseconds);
	return TimerString;
}

