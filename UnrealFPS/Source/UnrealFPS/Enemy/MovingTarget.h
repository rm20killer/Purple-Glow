// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MovingTarget.generated.h"

UCLASS()
class UNREALFPS_API AMovingTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector Startingpoint;
	UPROPERTY(EditAnywhere, Category = "Moving")
	float MovingTargetSpeed;
	UPROPERTY(EditAnywhere, Category = "Moving")
	FVector Endpoint;
	void TargetHit();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* CollisionComponent;
	bool MovingBackToStart = false;

};
