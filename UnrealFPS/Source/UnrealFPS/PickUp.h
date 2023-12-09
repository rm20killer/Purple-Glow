// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

UCLASS()
class UNREALFPS_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickUp")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickUp")
	float PickupRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickUp")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickUp")
	UStaticMeshComponent* PickupMeshComponent;
	
	
	
};
