// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

#include "Weapon.h"
#include "UnrealFPS/Enemy/MovingTarget.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.0f;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision profile name to "Projectile".
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// Event called when component hits something.
		CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
			TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 5000.0f;
		ProjectileMovementComponent->MaxSpeed = 5000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.0f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent)
	{
		//ProjectileMeshComponent -> CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));

		//static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/Player/GunModels/M_Bullet.M_Bullet"));
		//if (Material.Succeeded())
		//{
		//	ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		//}
		//
		//ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
		//ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
		//ProjectileMeshComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * when the projectile hits something add impulse to the object
 * check if the object is a moving target and call the target hit function
 * destroy the projectile
 * @param HitComponent The component that was hit
 * @param OtherActor the actor that was hit
 * @param OtherComp 
 * @param NormalImpulse 
 * @param Hit if the projectile hit something
 */
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	//Debug
	// UE_LOG(LogTemp, Warning, TEXT("Projectile hit object: "));
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("Projectile hit component: "));
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName());
	// FName OtherCompCollisionProfileName = OtherComp->GetCollisionProfileName();
	// UE_LOG(LogTemp, Warning, TEXT("Projectile hit component collision profile name: "));
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherCompCollisionProfileName.ToString());

	if (OtherActor != this && HitComponent->IsSimulatingPhysics())
	{
		HitComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	if (OtherActor != this)
	{
		//cast to MovingTarget
		AMovingTarget* MovingTarget = Cast<AMovingTarget>(OtherActor);
		if (MovingTarget)
		{
			MovingTarget->TargetHit();
		}
	}

	Destroy();
}

/**
 * Add velocity to the projectile in the given direction
 * @param ShootDirection the direction the projectile will be fired in
 */
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

/**
 * Add velocity to the projectile in the given direction
 * @param ShootDirection the direction the projectile will be fired in
 * @param Damage Change the damage of the projectile to the given value
 */
void AFPSProjectile::FireInDirection(const FVector& ShootDirection, const float Damage)
{
	this->BaseDamage = Damage;
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

/**
 * Add velocity to the projectile in the given direction
 * @param ShootDirection the direction the projectile will be fired in
 * @param Damage Change the damage of the projectile to the given value
 * @param ProjectileSpeed change the speed of the projectile to the given value
 */
void AFPSProjectile::FireInDirection(const FVector& ShootDirection, const float Damage, const float ProjectileSpeed)
{
	this->BaseDamage = Damage;
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
