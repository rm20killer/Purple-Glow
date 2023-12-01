// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSProjectile.h"
#include "Components/ArrowComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Weapon.generated.h"

UCLASS()
class UNREALFPS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//UCameraComponent* FPSCameraComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AFPSProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UStaticMeshComponent* FPSGunMesh;

	//Arrow component to specify the muzzle's location
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FVector MuzzleOffset_;
	//class UArrowComponent* ArrowLocation;

	void Fire(FVector CameraLocation, FRotator CameraRotation, APawn* PawnInstigator);
};
