// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALFPS_API UHealthSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector MovementVelocity;

	float HealthTweenDirection;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	 
	float Health;

	UFUNCTION(BlueprintCallable, Category = "HealthSystem")
	float GetHealth() const { return Health; }
	void SetHealth(float val) { Health = val; }
	UFUNCTION(BlueprintCallable, Category = "HealthSystem")
	float GetMaxHealth() const { return MaxHealth; }
	void SetMaxHealth(float val) { MaxHealth = val; }

	void TakeDamage(float DamageAmount);
	void AddHealth(float HealAmount);
	
};
