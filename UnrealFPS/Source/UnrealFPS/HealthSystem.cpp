// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSystem.h"

// Sets default values for this component's properties
UHealthSystem::UHealthSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;
	Health = MaxHealth;
}


// Called when the game starts
void UHealthSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthSystem::TakeDamage()
{
}
