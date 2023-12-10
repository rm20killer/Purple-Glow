// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Health_PickUp.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFPS_API AHealth_PickUp : public APickUp
{
	GENERATED_BODY()
public:
	AHealth_PickUp();
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Health" )
	float HealthAmount;
	
	virtual void OnOverlapBegin( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult ) override;
};
