// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedural_Wall.h"

#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AProcedural_Wall::AProcedural_Wall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WallLength = 5;
	WallHeight = 5;
	WallWidth = 1;
	Spacing = 100.0f;
	MaxXOffset = 0.0f;
	MaxYOffset = 0.0f;
	MaxZOffset = 0.0f;
	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MyMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	WallMesh = MyMesh.Object;
	WallMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallMesh"));
	WallMeshComponent2 = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallMesh2"));
	//make sure wallmeshCOmpoent is 0,0,0
	WallMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//set the material of the mesh
	if(WallMaterial)
	{
		WallMeshComponent->SetMaterial(0, WallMaterial);
	}
	if(WallMaterial2)
	{
		WallMeshComponent2->SetMaterial(0, WallMaterial2);
	}
	else
	{
		WallMeshComponent2->SetMaterial(0, WallMaterial);
	}
	//add a static mesh component
}

void AProcedural_Wall::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	//destroy all instances
	WallMeshComponent->ClearInstances();
	WallMeshComponent2->ClearInstances();
	CreateWall();
}
// Called when the game starts or when spawned
void AProcedural_Wall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProcedural_Wall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProcedural_Wall::CreateWall()
{
	//set the mesh to the static mesh component
	WallMeshComponent->SetStaticMesh(WallMesh);
	WallMeshComponent2->SetStaticMesh(WallMesh);
	//make WallMeshComponent2 a child of WallMeshComponent so that it moves with it
	WallMeshComponent2->AttachToComponent(WallMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// WallMeshComponent2->SetupAttachment(WallMeshComponent);
	if(WallMaterial)
	{
		WallMeshComponent->SetMaterial(0, WallMaterial);
	}
	if(WallMaterial2)
	{
		WallMeshComponent2->SetMaterial(0, WallMaterial2);
	}
	else
	{
		WallMeshComponent2->SetMaterial(0, WallMaterial);
	}
	//attach the static mesh component to the root component
	RootComponent = WallMeshComponent;
	for (int32 i = 0; i < WallLength; i++)
	{
		for (int32 j = 0; j < WallHeight; j++)
		{
			for (int32 k = 0; k < WallWidth; k++)
			{
				float XOffset = FMath::RandRange(-MaxXOffset, MaxXOffset);
				float YOffset = FMath::RandRange(-MaxYOffset, MaxYOffset);
				float ZOffset = FMath::RandRange(-MaxZOffset, MaxZOffset);
				float x = i * Spacing + XOffset;
				float y = k * Spacing + YOffset;
				float z = j * Spacing + ZOffset;
				FVector Location = FVector(x, y, z);
				if(FMath::RandRange(0, 100) < 10)
				{
					WallMeshComponent2->AddInstance(FTransform(Location));
				}
				else
				{
					WallMeshComponent->AddInstance(FTransform(Location));
				}
			}
		}
	}

}

void AProcedural_Wall::SetWall(int Length, int Height, int Width)
{
	WallLength = Length;
	WallHeight = Height;
	WallWidth = Width;
	WallMeshComponent->ClearInstances();
	WallMeshComponent2->ClearInstances();
	CreateWall();
}

void AProcedural_Wall::ResetWall()
{
	WallMeshComponent->ClearInstances();
	WallMeshComponent2->ClearInstances();
	CreateWall();
}

