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
	bTapFire = false;
	BaseDamage = 10.0f;
	ShotBeforeRecoil = 5;
	MaxRecoilPitch = 5.0f;
	MaxRecoilYaw = 5.0f;
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
		FPSGunMesh->CastShadow = false;
	}
	Ammo = MaxAmmo;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanShot == false)
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
		if(bTapFire == false)
		{
			GetWorldTimerManager().SetTimer(ShotHandle, this, &AWeapon::Fire, FireRate, true);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ShotHandle);
		//reset recoil
		shotFired = 0;
	}
}

/**
 * Check if the ammo is empty and if not fire a projectile
 * add recoil to the gun
 * Fire a projectile in the direction the gun is pointing
 */
void AWeapon::Fire()
{
	//check if ammo is empty or if the player is reloading
	if (bCanShot == false)
	{
		return;
	}
	if (Ammo > 0)
	{
		--Ammo;
		UE_LOG(LogTemp, Warning, TEXT("Ammo: %d"), Ammo);
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
		shotFired++;
		const FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		//add recoil by rotating the gun by a random amount on the pitch axis
		if(shotFired < ShotBeforeRecoil)
		{
			RecoilPitch = 0.2f;
			RecoilYaw = 0.2f;
		}
		else if(shotFired >= ShotBeforeRecoil)
		{
			//increase recoil until the recoil is at the max
			if (RecoilPitch < MaxRecoilPitch)
			{
				RecoilPitch += 1.2f;
			}
			if (RecoilYaw < MaxRecoilYaw)
			{
				RecoilYaw += 1.2f;
			}
			
		}
			MuzzleRotation.Pitch += FMath::RandRange(-RecoilPitch, RecoilPitch);
			MuzzleRotation.Yaw += FMath::RandRange(-RecoilYaw, RecoilYaw);
		
		//shot
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
	const FQuat QuatRotation = FQuat(FRotator(0, 0, 20.0f));

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
	if (Ammo == MaxAmmo || bCanShot == false)
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



//SETTERS AND GETTERS are below
void AWeapon::SetCanShot(bool bNewCanShot)
{
	this->bCanShot = bNewCanShot;
}

void AWeapon::SetAmmo(int NewAmmo)
{
	this->Ammo = NewAmmo;
	if (Ammo > MaxAmmo)
	{
		this->Ammo = MaxAmmo;
	}
}

void AWeapon::SetAmmo(int NewAmmo, bool bBypassMax)
{
	if (bBypassMax)
	{
		this->Ammo = NewAmmo;
	}
	else
	{
		SetAmmo(NewAmmo);
	}
}

void AWeapon::SetMaxAmmo(int NewMaxAmmo)
{
	this->MaxAmmo = NewMaxAmmo;
}

void AWeapon::SetFireRate(float NewFireRate)
{
	this->FireRate = NewFireRate;
}

int AWeapon::GetAmmo()
{
	return Ammo;
}

int AWeapon::GetMaxAmmo()
{
	return MaxAmmo;
}

FString AWeapon::GetAmmoString()
{
	FString AmmoString = FString::Printf(TEXT("Ammo: %d/%d"), Ammo, MaxAmmo);
	return AmmoString;
}