// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Key_PickUp.generated.h"

/**
 * 
 */
UCLASS()

//a key pickup that can be used to open a door
class UNREALFPS_API AKey_PickUp : public APickUp
{
	GENERATED_BODY()
public:
	AKey_PickUp();
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "KeyID" )
	int32 KeyID;
	
	virtual void OnOverlapBegin( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult ) override;
};
