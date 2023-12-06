// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "Kismet/KismetMathLibrary.h"
#include "Player/MainPlayerController.h"

// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireRate = 0.1f;
	MaxAmmo = 10;
	bTapFire = false;
	BaseDamage = 2.0f;
	ShotBeforeRecoil = 2;
	MaxRecoilPitch = 7.5f;
	MaxRecoilYaw = 7.5f;
	MuzzleOffset.Set(80.0f, 200.0f, -20.0f);
	TurrertLocation = FVector(0.0f, 0.0f, 40.0f);
	TurrertRotation = FRotator(0.0f, 0.0f, 0.0f);
	RecoilPitch = 1.0f;
	RecoilYaw = 1.0f;
	ReloadTime = 1.5f;
	MaxSearchRotation = 45.0f;

	//create the turret base mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretBaseMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Enemy/Turret_base.Turret_base'"));
	TurretBaseMesh = TurretBaseMeshAsset.Object;
	//get root relative location
	TurretBaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBaseMesh"));
	TurretBaseMeshComponent->SetStaticMesh(TurretBaseMesh);
	TurretBaseMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TurretBaseMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	//make root component
	RootComponent = TurretBaseMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretGunMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Enemy/Turret_Gun.Turret_Gun'"));
	TurretGunMesh = TurretGunMeshAsset.Object;
	TurretGunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGunMesh"));
	TurretGunMeshComponent->SetStaticMesh(TurretGunMesh);
	TurretGunMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	TurretGunMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	TurretGunMeshComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurrertLocation = TurretGunMeshComponent->GetComponentLocation()+FVector(0.0f, 10.0f, 30.0f);
	TurrertRotation = TurretGunMeshComponent->GetComponentRotation();
	if(bSearching)
	{
		Search();
		UE_LOG(LogTemp, Warning, TEXT("Searching"))
	}
	else if(bAiming)
	{
		Aim();
		UE_LOG(LogTemp, Warning, TEXT("Aiming"))
	}
}

void ATurret::Search()
{
	FRotator NewRotation = TurrertRotation;
	//rotate the TurretGunMesh if past max rotation rotate in the opposite direction
	if (TurrertRotation.Yaw >= MaxSearchRotation)
	{
		NewRotation.Yaw -= 2.5f;
	}
	else if (TurrertRotation.Yaw <= -MaxSearchRotation)
	{
		NewRotation.Yaw += 2.5f;
	}
	TurretGunMeshComponent->AddRelativeRotation(NewRotation);
	TurrertRotation = TurretGunMeshComponent->GetComponentRotation();
	//create a cone trace for the turret
	FCollisionQueryParams TraceParams(FName(TEXT("TurretTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(this);
	FVector StartLocation = TurrertLocation;
	//foward vector is where the TurrertRotation is pointing
	FVector ForwardVector = TurrertRotation.Vector();
	FVector EndLocation = ForwardVector * 10000.0f + StartLocation;
	
	//draw the line trace
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.3f, 0.0f, 1.0f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	
	if (HitResult.bBlockingHit)
	{
		//get the hit actor
		HitActor = HitResult.GetActor();
		//if the hit actor is not null
		if (HitActor != nullptr)
		{
			//if cast to player is successful
			if (Cast<AMainPlayerController>(HitActor))
			{
				UE_LOG( LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName() );
				//set the turret's rotation to look at the player
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitActor->GetActorLocation()));
				//set the turret's state to aiming
				bSearching = false;
				bAiming = true;
			}
		}
	}
	
}

void ATurret::Aim()
{
	//if the hit actor is not null
	if (HitActor != nullptr)
	{
		//if cast to player is successful
		if (Cast<AMainPlayerController>(HitActor))
		{
			//slowly move the turret's rotation to look at the player
			TurrertRotation = FMath::RInterpTo(TurrertRotation, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitActor->GetActorLocation()), GetWorld()->GetDeltaSeconds(), RotationSpeed);
			TurretGunMeshComponent->SetRelativeRotation(TurrertRotation);
			//set the turret's state to aiming
			bSearching = false;
			bAiming = true;

			//if the turret is aimed at the player
			if (TurrertRotation.Equals(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitActor->GetActorLocation()), 1.0f))
			{
				//set the turret's state to firing
				bAiming = false;
				bFiring = true;
				Shot(true);
			}
		}
	}
	else
	{
		bSearching = true;
		bAiming = false;
	}
}

void ATurret::Shot(bool bFire)
{
	if (bFire)
	{
		GetWorldTimerManager().SetTimer(ShotHandle, this, &ATurret::Fire, FireRate, true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ShotHandle);
		shotFired = 0;
	}
}

void ATurret::Fire()
{
	if (bCanShot == false)
	{
		return;
	}
	if (Ammo > 0)
	{
		--Ammo;
		UE_LOG(LogTemp, Warning, TEXT("Turret Ammo: %d"), Ammo);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ShotHandle);
		Reload();
		return;
	}

	if (ProjectileClass)
	{
		shotFired++;
		const FVector MuzzleLocation = TurrertLocation + FTransform(TurrertRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = TurrertRotation;
		if (shotFired < ShotBeforeRecoil)
		{
			RecoilPitch = 1.0f;
			RecoilYaw = 1.0f;
		}
		else if (shotFired >= ShotBeforeRecoil)
		{
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
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			if (AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(
				ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams))
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void ATurret::Reload()
{
	if (Ammo == MaxAmmo || bCanShot == false)
	{
		return;
	}
	bCanShot = false;
	GetWorldTimerManager().ClearTimer(ShotHandle);
	GetWorldTimerManager().SetTimer(ReloadHandler, this, &ATurret::Reloaded, 0.5f, false);
}

void ATurret::Reloaded()
{
	Ammo = MaxAmmo;
	bCanShot = true;
	bFiring = false;
	bSearching = true;
	bAiming = false;
}
