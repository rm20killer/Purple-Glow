// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UnrealFPS/Player/HealthSystem.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
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
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("EnemyProjectile"));
		// Event called when component hits something.
		CollisionComponent->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);
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
		ProjectileMovementComponent->InitialSpeed = 4000.0f;
		ProjectileMovementComponent->MaxSpeed = 4000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.0f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	BaseDamage = 5.0f;
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * When the projectile hits something, add an impulse to the object that was hit
 * if it has health system, take damage to that object
 * @param HitComponent The component that was hit
 * @param OtherActor the actor that was hit
 * @param OtherComp 
 * @param NormalImpulse 
 * @param Hit if the projectile hit something
 */
void AEnemyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	// UE_LOG(LogTemp, Warning, TEXT("Projectile hit object: "));
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("Projectile hit component: "));
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetName());
	// FName OtherCompCollisionProfileName = OtherComp->GetCollisionProfileName();
	// UE_LOG(LogTemp, Warning, TEXT("Projectile hit component collision profile name: "));
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherCompCollisionProfileName.ToString());

	if (OtherActor != this && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 50.0f, GetActorLocation());
	}
	UHealthSystem* HealthSystem = Cast<UHealthSystem>(OtherActor->GetComponentByClass(UHealthSystem::StaticClass()));
	if (OtherActor != this && HealthSystem != nullptr)
	{
		HealthSystem->TakeDamage(BaseDamage);
	}

	Destroy();
}

/**
 * Add velocity to the projectile in the given direction
 * @param ShootDirection the direction the projectile will be fired in
 */
void AEnemyProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
