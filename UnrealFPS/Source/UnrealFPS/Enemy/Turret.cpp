// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "Kismet/KismetMathLibrary.h"
#include "../Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

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
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
	TurrertLocation = FVector(0.0f, 0.0f, 40.0f);
	TurrertRotation = FRotator(0.0f, 0.0f, 0.0f);
	RecoilPitch = 1.0f;
	RecoilYaw = 1.0f;
	ReloadTime = 1.5f;
	MaxSearchRotation = 45.0f;

	//create the turret base mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretBaseMeshAsset(
		TEXT("/Script/Engine.StaticMesh'/Game/Enemy/Turret_base.Turret_base'"));
	TurretBaseMesh = TurretBaseMeshAsset.Object;
	//get root relative location
	TurretBaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBaseMesh"));
	TurretBaseMeshComponent->SetStaticMesh(TurretBaseMesh);
	TurretBaseMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TurretBaseMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	TurretBaseMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("EnemyProjectile"));
	//make root component
	RootComponent = TurretBaseMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretGunMeshAsset(
		TEXT("/Script/Engine.StaticMesh'/Game/Enemy/Turret_Gun.Turret_Gun'"));
	TurretGunMesh = TurretGunMeshAsset.Object;
	TurretGunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGunMesh"));
	TurretGunMeshComponent->SetStaticMesh(TurretGunMesh);
	TurretGunMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	TurretGunMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	TurretGunMeshComponent->SetupAttachment(RootComponent);
	TurretGunMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("EnemyProjectile"));
	//create cone mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMeshAsset(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	ConeMesh = ConeMeshAsset.Object;
	ConeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConeMesh"));
	ConeMeshComponent->SetStaticMesh(ConeMesh);
	ConeMeshComponent->SetupAttachment(TurretGunMeshComponent);
	ConeMeshComponent->SetRelativeLocation(FVector(580.0f, 0.0f, 20.0f));
	ConeMeshComponent->SetRelativeRotation(FRotator(90.0f, 360.0f, 360.0f));
	ConeMeshComponent->SetRelativeScale3D(FVector(6.0f, 6.0f, 10.0f));
	ConeMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("TurretScan"));
	//overlap event
	ConeMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	static ConstructorHelpers::FObjectFinder<UMaterial> ConeMaterialAsset(
		TEXT("/Script/Engine.Material'/Game/Enemy/M_SeeThrough.M_SeeThrough'"));
	ConeMeshComponent->SetMaterial(0, ConeMaterialAsset.Object);
	ConeMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	DefaultRotation = GetActorRotation();
	Super::BeginPlay();
	//set the turret's state to searching
	bSearching = true;
	bAiming = false;
	bFiring = false;
	//start rotating the turret
	// GetWorldTimerManager().SetTimer(RotationHandler, this, &ATurret::RotateGun, 0.05f, true);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaTimeForTick = DeltaTime;
	TurrertLocation = TurretGunMeshComponent->GetComponentLocation() + FVector(0.0f, 10.0f, 15.0f);
	TurrertRotation = TurretGunMeshComponent->GetRelativeRotation();
	if (bSearching)
	{
		// Search();
		if (bRotatingLeft)
		{
			TurrertRotation.Yaw = FMath::Max(TurrertRotation.Yaw - SearchRotationSpeed, -MaxSearchRotation);

			if (TurrertRotation.Yaw <= -MaxSearchRotation)
			{
				bRotatingLeft = false;
			}
		}
		else
		{
			TurrertRotation.Yaw = FMath::Min(TurrertRotation.Yaw + SearchRotationSpeed, MaxSearchRotation);

			if (TurrertRotation.Yaw >= MaxSearchRotation)
			{
				bRotatingLeft = true;
			}
		}
		TurretGunMeshComponent->SetRelativeRotation(TurrertRotation);
		// UE_LOG(LogTemp, Warning, TEXT("Searching"))
	}
	else if (bAiming)
	{
		Aim(DeltaTime);
		// UE_LOG(LogTemp, Warning, TEXT("Aiming"))
	}
}

void ATurret::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if the turret is searching
	if (bSearching)
	{
		UE_LOG(LogTemp, Warning, TEXT("Turret Detected: %s"), *OtherActor->GetName());
		//check if the overlapped actor is the player
		if (Cast<AMainPlayerController>(OtherActor))
		{
			//slow down the turret's rotation
			SearchRotationSpeed = SearchRotationSpeed / 2.0f;
			//cast an ray trace to the player to check if the player is not behind a wall
			FHitResult HitResult;
			FCollisionQueryParams TraceParams(FName(TEXT("TurretTrace")), false, this);
			TraceParams.bReturnPhysicalMaterial = false;
			FVector StartLocation = TurrertLocation;
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(
				StartLocation, OtherActor->GetActorLocation());
			FVector EndLocation = Direction * 10000.0f + StartLocation;
			GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn, TraceParams);
			//draw ray trace make it blue if it hit something and red if it didn't
			// DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red,false, 5.0f, 0, 10.0f);

			//if the ray trace hit the player
			if (HitResult.bBlockingHit)
			{
				//get the hit actor and check if the hit actor is not null
				if (AActor* HitResultActor = HitResult.GetActor(); HitResultActor != nullptr)
				{
					//if cast to player is successful
					if (Cast<AMainPlayerController>(HitResultActor))
					{
						SearchRotationSpeed = SearchRotationSpeed * 2.0f;
						UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResultActor->GetName());
						//set the turret's state to aiming
						bSearching = false;
						bAiming = true;
						//stop rotating the turret
						// GetWorldTimerManager().ClearTimer(RotationHandler);
						HitActor = HitResultActor;
					}
				}
				else
				{
					SearchRotationSpeed = SearchRotationSpeed * 2.0f;
				}
			}
			else
			{
				SearchRotationSpeed = SearchRotationSpeed * 2.0f;
			}
		}
	}
}

void ATurret::RotateGun()
{
	// bool bRotatingLeft = (TurrertRotation.Yaw >= MaxSearchRotation);
	// if (bRotatingLeft)
	// {
	// 	TurrertRotation.Yaw -= SearchRotationSpeed;
	// }
	// else
	// {
	// 	TurrertRotation.Yaw += SearchRotationSpeed;
	// }
	// // TurrertRotation.Yaw += roationSpeed;
	// TurretGunMeshComponent->SetRelativeRotation(TurrertRotation);
}

/**
 * DO NOT USE 
 */
void ATurret::Search()
{
	return;
	// //create a cone trace for the turret
	// // UE_LOG(LogTemp, Warning, TEXT("New TurretRotation: %s"), *TurrertRotation.ToString());
	// FCollisionQueryParams TraceParams(FName(TEXT("TurretTrace")), true, this);
	// TraceParams.bTraceComplex = true;
	// TraceParams.bReturnPhysicalMaterial = false;
	// TraceParams.AddIgnoredActor(this);
	// //draw cone trace
	// FVector StartLocation = TurrertLocation;
	// FVector Direction = TurrertRotation.Vector();
	// FVector EndLocation = Direction * 10000.0f + StartLocation;
	//
	//
	// if (HitResult.bBlockingHit)
	// {
	// 	//get the hit actor
	// 	HitActor = HitResult.GetActor();
	// 	//if the hit actor is not null
	// 	if (HitActor != nullptr)
	// 	{
	// 		//if cast to player is successful
	// 		if (Cast<AMainPlayerController>(HitActor))
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
	// 			//set the turret's rotation to look at the player
	// 			SetActorRotation(
	// 				UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitActor->GetActorLocation()));
	// 			//set the turret's state to aiming
	// 			bSearching = false;
	// 			bAiming = true;
	// 			//stop rotating the turret
	// 			GetWorldTimerManager().ClearTimer(RotationHandler);
	// 		}
	// 	}
	// }
}

void ATurret::Aim(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("Aiming"));
	//if the hit actor is not null
	if (HitActor != nullptr)
	{
		//if cast to player is successful
		if (Cast<AMainPlayerController>(HitActor))
		{
			FHitResult HitResult;
			FCollisionQueryParams TraceParams(FName(TEXT("TurretTrace")), false, this);
			TraceParams.bReturnPhysicalMaterial = false;
			FVector StartLocation = TurrertLocation;
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(
				StartLocation, HitActor->GetActorLocation());
			FVector EndLocation = Direction * 10000.0f + StartLocation;
			GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn, TraceParams);
			// DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
			// check if the ray trace hit something and if the hit actor is not the player
			if (HitResult.bBlockingHit && HitResult.GetActor() != HitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
				//set the turret's state to searching
				bSearching = true;
				bAiming = false;
				bFiring = false;
				TurrertRotation = FRotator(0.0f, TurrertRotation.Yaw, 0.0f);
				TurretGunMeshComponent->SetRelativeRotation(TurrertRotation);
				return;
			}
			FVector BarrelForwardVector = TurretGunMeshComponent->GetForwardVector();

			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation() + BarrelForwardVector, HitActor->GetActorLocation() + FVector(0.0f, 0.0f, -30.0f));
			// UE_LOG(LogTemp, Warning, TEXT("LookAtRotation: %s"), *LookAtRotation.ToString());
			// LookAtRotation.Yaw=LookAtRotation.Yaw-DefaultRotation.Yaw;
			//slowly move the turret's rotation to look at the player
			TurrertRotation = FMath::RInterpTo(TurretGunMeshComponent->GetComponentRotation(), LookAtRotation,
			                                   DeltaTime, RotationSpeed);
			TurretGunMeshComponent->SetWorldRotation(TurrertRotation);
			// UE_LOG(LogTemp, Warning, TEXT("TurretRotation: %s"), *TurrertRotation.ToString());
			//if the turret is aimed at the player
			if (TurrertRotation.Equals(LookAtRotation, 1.0f))
			{
				//set the turret's state to firing
				bAiming = false;
				bFiring = true;
				Shot(true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Going back to searching"));
			bSearching = true;
			bAiming = false;
			bFiring = false;
			//start rotating the turret
			// GetWorldTimerManager().SetTimer(RotationHandler, this, &ATurret::RotateGun, 0.05f, true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Going back to searching"));
		bSearching = true;
		bAiming = false;
		bFiring = false;
		//start rotating the turret
		// GetWorldTimerManager().SetTimer(RotationHandler, this, &ATurret::RotateGun, 0.05f, true);
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
		//reset recoil
		shotFired = 0;
		bSearching = true;
		bAiming = false;
		bFiring = false;
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
		FVector BarrelForwardVector = TurretGunMeshComponent->GetForwardVector();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation() + BarrelForwardVector, HitActor->GetActorLocation() + FVector(0.0f, 0.0f, -30.0f));
		TurrertRotation = FMath::RInterpTo(TurretGunMeshComponent->GetComponentRotation(), LookAtRotation,DeltaTimeForTick, RotationSpeed);
		TurretGunMeshComponent->SetWorldRotation(TurrertRotation);

		
		shotFired++;
		const FVector MuzzleLocation = TurrertLocation + FTransform(TurretGunMeshComponent->GetComponentRotation()).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = TurretGunMeshComponent->GetComponentRotation();;
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
		UE_LOG(LogTemp, Warning, TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString());
		MuzzleRotation.Pitch += FMath::RandRange(-RecoilPitch, RecoilPitch);
		MuzzleRotation.Yaw += FMath::RandRange(-RecoilYaw, RecoilYaw);
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			if (AEnemyProjectile* Projectile = World->SpawnActor<AEnemyProjectile>(
				ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams))
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				//play fire sound
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Projectile is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("World is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is null"));
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
	// GetWorldTimerManager().SetTimer(RotationHandler, this, &ATurret::RotateGun, 0.05f, true);
	//reset recoil
	shotFired = 0;
	Ammo = MaxAmmo;
	bCanShot = true;
	bFiring = false;
	bSearching = true;
	bAiming = false;

	//put the turret rotation back to the original rotation
	TurrertRotation = FRotator(0.0f, TurrertRotation.Yaw, 0.0f);
	TurretGunMeshComponent->SetRelativeRotation(TurrertRotation);
}
