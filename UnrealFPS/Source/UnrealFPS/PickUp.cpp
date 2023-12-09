// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"

#include "Player/MainPlayerController.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PickUpSceneComponent"));
	}
	PickupRange = 100.0f;
	RotationSpeed = 50.0f;
	if(!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("PickUp"));
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickUp::OnOverlapBegin);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		CollisionComponent->InitSphereRadius(PickupRange);
		RootComponent = CollisionComponent;
	}

}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->InitSphereRadius(PickupRange);
	
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//rotate the pickup around the z axis
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += DeltaTime * RotationSpeed;
	SetActorRotation(Rotation);
}

void APickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor picked up %s"), *OtherActor->GetName());
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(OtherActor);
	if(PlayerController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Overlap"));
	}
}


