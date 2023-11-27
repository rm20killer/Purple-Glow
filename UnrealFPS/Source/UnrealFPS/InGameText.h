// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/TextRenderComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InGameText.generated.h"

UCLASS()
class UNREALFPS_API AInGameText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInGameText();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objects/Text")
	UTextRenderComponent* MyText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objects/Text")
	FString TextString;
};
