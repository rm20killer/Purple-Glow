// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values for this component's properties
UWeapon::UWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (FPSGunMesh != nullptr)
	{
		FPSGunMesh->bCastDynamicShadow = false;

		FPSGunMesh->CastShadow = false;

		// Only the owning player sees this mesh.
		FPSGunMesh->SetOnlyOwnerSee(true);
	}

	//Get the camera component
	FPSCameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();

	
}


// Called every frame
void UWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeapon::Fire()
{
	return;

	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		//get ActorEyesViewPoint from the player controller which is attached to the pawn
		//GetOwner()->GetActorEyesViewPoint(CameraLocation, CameraRotation);

		
		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetOwner()->GetInstigator();

			// Spawn the projectile at the muzzle.
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}

}