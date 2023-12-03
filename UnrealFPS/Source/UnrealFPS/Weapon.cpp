// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	FPSGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FPSGunMesh"));
	FPSGunMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FPSGunMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	FPSGunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//set the mesh to the root component
	RootComponent = FPSGunMesh;
	FireRate = 0.1f;
	MaxAmmo = 30;
	MuzzleOffset.Set(80.0f, 200.0f, -20.0f);
	CameraLocation = FVector(0.0f, 0.0f, 0.0f);
	CameraRotation = FRotator(0.0f, 0.0f, 0.0f);
	PawnInstigator = nullptr;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (FPSGunMesh != nullptr)
	{
		FPSGunMesh->bCastDynamicShadow = false;
		FPSGunMesh->CastShadow = false;
	}
	Ammo = MaxAmmo;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bCanShot == false)
	{
		ReloadAnimation();
	}
}


/**
 * Check if the fire button is pressed
 * @param bFireStart start or stop firing
 * @param cameraLocation location of the camera
 * @param cameraRotation rotation of the camera
 * @param pawnInstigator the player
 */
void AWeapon::Shot(bool bFireStart, FVector& cameraLocation, FRotator& cameraRotation, APawn* pawnInstigator)
{
	CameraLocation.Set(cameraLocation.X, cameraLocation.Y, cameraLocation.Z);
	CameraRotation = cameraRotation;
	PawnInstigator = pawnInstigator;
	if (bFireStart)
	{
		Fire();
		GetWorldTimerManager().SetTimer(ShotHandle, this, &AWeapon::Fire, FireRate, true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ShotHandle);
	}
}

/**
 * Check if the ammo is empty and if not fire a projectile
 * Fire a projectile in the direction the gun is pointing
 */
void AWeapon::Fire()
{
	//check if ammo is empty or if the player is reloading
	if(bCanShot == false)
	{
		return;
	}
	if(Ammo > 0)
	{
		--Ammo;
		UE_LOG(LogTemp, Warning, TEXT("Ammo: %d"), Ammo);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ammo: %d"), Ammo);
		
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ShotHandle);
		Reload();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Shot called"));

	if (ProjectileClass)
	{
		const FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		const FRotator MuzzleRotation = CameraRotation;
		//MuzzleRotation.Pitch += 10.0f;
		
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = PawnInstigator;
			if (AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(
				ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams))
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

/**
 * rotate the gun on the roll axis to simulate an animation
 */
void AWeapon::ReloadAnimation()
{
	//spin the gun on roll axis by 5 degree per frame
	const FQuat QuatRotation = FQuat(FRotator(0, 0, 5.0f));
	
	FPSGunMesh->AddRelativeRotation(QuatRotation, false, 0, ETeleportType::None);

	// Check if the gun has rotated 360 degrees on the roll axis
	if (FMath::Abs(GetActorRotation().Roll) >= 360.0f)
	{
		FPSGunMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}

/**
 * check if the ammo is full and if not play the reload animation and wait 0.5 seconds
 */
void AWeapon::Reload()
{
	//check if ammo is full
	if (Ammo == MaxAmmo)
	{
		return;
	}
	bCanShot = false;
	//play reload animation
	ReloadAnimation();
	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
	//wait 0.5 seconds then carry on
	GetWorldTimerManager().ClearTimer(ShotHandle);
	GetWorldTimerManager().SetTimer(reloadHandler, this, &AWeapon::Reloaded, 0.5f, false);
}

/**
 * reset the animation and allow the player to fire again
 */
void AWeapon::Reloaded()
{
	//set ammo to max and allow the player to fire again
	//reset the gun rotation
	UE_LOG(LogTemp, Warning, TEXT("Reloaded"));
	const FQuat QuatRotation = FQuat(FRotator(0.0f, 0.0f, 0.0f));
	FPSGunMesh->SetRelativeRotation(QuatRotation, false, 0, ETeleportType::None);
	Ammo = MaxAmmo;
	bCanShot = true;
}

/**
 * update the location of the camera
* @param cameraLocation location of the camera
 * @param cameraRotation rotation of the camera
 */
void AWeapon::UpdateLoc(FVector& cameraLocation, FRotator& cameraRotation)
{
	CameraLocation.Set(cameraLocation.X, cameraLocation.Y, cameraLocation.Z);
	CameraRotation = cameraRotation;
}
