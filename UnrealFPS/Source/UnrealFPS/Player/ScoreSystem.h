// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALFPS_API UScoreSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UScoreSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 Score;
	int32 HighScore;
	float TotalTimeInSeconds;
	// int64 TimeInMilliseconds;
	float TimeInSeconds;
	bool bIsTimerActive;
	bool bIsPlatinum;
	bool bIsGold;
	bool bIsSilver;
	bool bIsBronze;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "ScoreSystem")
	float BrozenTime = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "ScoreSystem")
	float SilverTime = 6.0f;
	UPROPERTY(EditDefaultsOnly, Category = "ScoreSystem")
	float GoldTime = 3.5f;
	UPROPERTY(EditDefaultsOnly, Category = "ScoreSystem")
	float PlatinumTime = 2.0f;

	void AddScore(int32 ScoreToAdd);
	void SaveScore();
	// void LoadScore();
	UFUNCTION(BlueprintCallable, Category = "ScoreSystem")
	int32 GetScore();
	UFUNCTION(BlueprintCallable, Category = "ScoreSystem")
	int32 GetHighScore();
	UFUNCTION(BlueprintCallable, Category = "ScoreSystem")
	int64 GetTotalTimeInSeconds();
	UFUNCTION(BlueprintCallable, Category = "ScoreSystem")
	FString GetTimerString();

	UFUNCTION(BlueprintCallable, Category = "ScoreSystem")
	void SetSore(int32 val) { Score = val; }

	UFUNCTION(BlueprintCallable, Category = "ScoreSystem")
	void SetHighScore(int32 val) { HighScore = val; }

	UFUNCTION(BlueprintCallable, Category = "ScoreSystem")
	void SetTotalTimeInSeconds(float val) { TotalTimeInSeconds = val; }
};
