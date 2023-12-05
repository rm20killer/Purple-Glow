// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSProjectile.h"
#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class UNREALFPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();
	void OnConstruction(const FTransform& Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//UCameraComponent* FPSCameraComponent;
	int Ammo = 10;
	bool bCanShot = true;
	UArrowComponent* ArrowComponent;
	FTimerHandle ShotHandle;
	FTimerHandle reloadHandler;
	FVector CameraLocation;
	FRotator CameraRotation;
	APawn* PawnInstigator;
	int shotFired = 0;
	float RecoilPitch;
	float RecoilYaw;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UStaticMeshComponent* FPSGunMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlash;

	//UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	//UUserWidget* AmmoWidget;

	void Shot(bool bFireStart, FVector& CameraLocation, FRotator& CameraRotation, APawn* PawnInstigator);
	void Fire();
	void ReloadAnimation();
	void Reload();
	void Reloaded();
	void UpdateLoc(FVector& CameraLocation, FRotator& CameraRotation);
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	int MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float FireRate;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	bool bTapFire;
	UPROPERTY( EditDefaultsOnly, Category = "WeaponStats")
	float BaseDamage;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float ReloadTime;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float MaxRecoilPitch;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float MaxRecoilYaw;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStats")
	float ShotBeforeRecoil;
	//setters
	void SetCanShot(bool bNewCanShot);
	void SetAmmo(int NewAmmo);
	void SetAmmo(int NewAmmo, bool bBypassMax);
	void SetMaxAmmo(int NewMaxAmmo);
	void SetFireRate(float NewFireRate);

	//getters
	int GetAmmo();
	int GetMaxAmmo();
	FString GetAmmoString();
};
