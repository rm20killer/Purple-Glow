// Fill out your copyright notice in the Description page of Project Settings.


#include "Key_PickUp.h"

#include "Player/MainPlayerController.h"

void AKey_PickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(OtherActor);
	if(PlayerController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Overlap"));
		PlayerController->Keys.AddUnique(KeyID);
		Destroy();
	}
}
