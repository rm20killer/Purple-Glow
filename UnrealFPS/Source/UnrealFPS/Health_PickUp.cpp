// Fill out your copyright notice in the Description page of Project Settings.


#include "Health_PickUp.h"

#include "Player/MainPlayerController.h"
AHealth_PickUp::AHealth_PickUp()
{
	PickupRange = 32.0f;
	HealthAmount = 25.0f;
}
void AHealth_PickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(OtherActor);
	if(PlayerController != nullptr)
	{
		PlayerController->HealthSystem->AddHealth(HealthAmount);
		Destroy();
	}
}
