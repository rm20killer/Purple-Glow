// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyProjectile.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class UNREALFPS_API ATurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//UCameraComponent* FPSCameraComponent;
	int Ammo = 10;
	bool bCanShot = true;
	// UArrowComponent* ArrowComponent;
	FTimerHandle ShotHandle;
	FTimerHandle ReloadHandler;
	FTimerHandle RotationHandler;
	FVector TurrertLocation;
	FRotator TurrertRotation;
	int shotFired = 0;
	float RecoilPitch;
	float RecoilYaw;
	bool bReloading = false;
	bool bSearching = true;
	bool bAiming = false;
	bool bFiring = false;
	bool bRotatingLeft = false;
	AActor* HitActor;
	UStaticMeshComponent* TurretBaseMeshComponent;
	UStaticMeshComponent* TurretGunMeshComponent;
	UStaticMeshComponent* ConeMeshComponent;
	UStaticMesh* ConeMesh;
	FRotator DefaultRotation;
	float DeltaTimeForTick;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RotateGun();
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AEnemyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UStaticMesh* TurretGunMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UStaticMesh* TurretBaseMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlash;

	void Search();
	void Aim(float DeltaTime);
	void Shot(bool bFire);
	void Fire();
	// void ReloadAnimation();
	void Reload();
	void Reloaded();
	
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	int MaxAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float FireRate;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	bool bTapFire;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float BaseDamage;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float ReloadTime;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float MaxRecoilPitch;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float MaxRecoilYaw;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float ShotBeforeRecoil;
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	float MaxSearchRotation;
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	float RotationSpeed = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	float SearchRotationSpeed = 2.0f;
};
