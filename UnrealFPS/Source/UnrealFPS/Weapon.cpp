// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//create a muzzle location component
	//ArrowLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowLocation"));
	//set arrow location to 0,0,0
	//ArrowLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//create a gun mesh component
	FPSGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FPSGunMesh"));
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();


	if (FPSGunMesh != nullptr)
	{
		FPSGunMesh->bCastDynamicShadow = false;

		FPSGunMesh->CastShadow = false;

		// Only the owning player sees this mesh.
		FPSGunMesh->SetOnlyOwnerSee(true);
	}


}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire(FVector CameraLocation, FRotator CameraRotation, APawn* PawnInstigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Shot called"));

	if (ProjectileClass)
	{

		//get MuzzleLocation from arrow component
		//MuzzleOffset_ = ArrowLocation->GetComponentLocation();


		MuzzleOffset_.Set(80.0f, 200.0f, -20.0f);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset_);
		FRotator MuzzleRotation = CameraRotation;
		//MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = PawnInstigator;
			//le log location and rotation
			UE_LOG(LogTemp, Warning, TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString());
			UE_LOG(LogTemp, Warning, TEXT("MuzzleRotation: %s"), *MuzzleRotation.ToString());
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

