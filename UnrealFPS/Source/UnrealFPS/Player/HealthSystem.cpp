// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSystem.h"

#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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

/**
 * decrease health by DamageAmount and check if health is less than 0
 * if health is less than 0, end the game
 * @param DamageAmount Damage that will be taken
 */
void UHealthSystem::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Health = 0;
		//TODO: Die
		//get owner
		AActor* Owner = GetOwner();
		if (Owner)
		{
			//pause the game and show cursor
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
			}
			//show game over widget
			AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(Owner);
			if (MainPlayerController)
			{
				MainPlayerController->Death();
			}
			// Owner->Destroy();
		}
	}
}

void UHealthSystem::AddHealth(float HealAmount)
{
	Health += HealAmount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}
