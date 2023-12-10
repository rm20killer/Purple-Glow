// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFPS_API UHealthbar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = ( BindWidget ))
	class UProgressBar* Healthbar;
	UPROPERTY(meta = ( BindWidget ))
	class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta = ( BindWidget ))
	class UTextBlock* MaxHealthLabel;
};
