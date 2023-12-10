// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedural_Room.h"

// Sets default values
AProcedural_Room::AProcedural_Room()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Spacing = 100.0f;
	MaxXOffset = 10.0f;
	MaxYOffset = 10.0f;
	MaxZOffset = 10.0f;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MyMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	WallMesh = MyMesh.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> MyMaterial(TEXT("/Script/Engine.Material'/Game/Objects/M_Walls.M_Walls'"));
	WallMaterial = MyMaterial.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> MyMaterial2(TEXT("/Script/Engine.Material'/Game/Objects/M_Walls_Glow.M_Walls_Glow'"));
	WallMaterial2 = MyMaterial2.Object;
	static  ConstructorHelpers::FObjectFinder<UMaterial> MyFloorMaterial(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	FloorMaterial = MyFloorMaterial.Object;
	
}

// Called when the game starts or when spawned
void AProcedural_Room::BeginPlay()
{
	Super::BeginPlay();
	CreateRoom();
}

// Called every frame
void AProcedural_Room::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * On a change in the editor, nuke the room and rebuild it
 * @param Transform 
 */
void AProcedural_Room::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	CreateRoom();
}
/**
 * check which walls are enabled and spawn them with the correct properties
 * Making sure to edit the old walls if they exist instead of spawning new ones
 * If a wall is disabled, destroy it if it exists
 */
void AProcedural_Room::CreateRoom()
{
	//check if all walls exist
	if(bLeftWall)
	{
		if(LeftWall == nullptr)
		{
			LeftWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
			LeftWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			LeftWall->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));	
		}
		LeftWall->WallMesh = this->WallMesh;
		LeftWall->WallMaterial = this->WallMaterial;
		LeftWall->WallMaterial2 = this->WallMaterial2;
		LeftWall->MaxYOffset = this->MaxYOffset;
		LeftWall->SetWall(RoomLength, RoomHeight, 1);
	}
	else
	{
		if(LeftWall != nullptr)
		{
			LeftWall->Destroy();
		}
	}
	
	if(bRightWall)
	{
		if(RightWall == nullptr)
		{
			
		RightWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
		RightWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		RightWall->SetActorRelativeLocation(FVector(0.0f, RoomWidth*(RightWall->Spacing-1), 0.0f));
		}
		RightWall->WallMesh = this->WallMesh;
		RightWall->WallMaterial = this->WallMaterial;
		RightWall->WallMaterial2 = this->WallMaterial2;
		RightWall->MaxYOffset = this->MaxYOffset;
		RightWall->SetWall(RoomLength, RoomHeight, 1);
	}
	else
	{
		if(RightWall != nullptr)
		{
			RightWall->Destroy();
		}
	}
	if(bFrontWall)
	{
		if(FrontWall == nullptr)
		{
			FrontWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
			FrontWall->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			FrontWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			FrontWall->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));	
		}
		FrontWall->WallMesh = this->WallMesh;
		FrontWall->WallMaterial = this->WallMaterial;
		FrontWall->WallMaterial2 = this->WallMaterial2;
		FrontWall->MaxYOffset = this->MaxXOffset;
		FrontWall->SetWall(RoomWidth, RoomHeight, 1);
	}
	else
	{
		if(FrontWall != nullptr)
		{
			FrontWall->Destroy();
		}
	}
	if(bBackWall)
	{
		if(BackWall== nullptr)
		{
			BackWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
			BackWall->SetActorRelativeLocation(FVector(RoomLength*(BackWall->Spacing-1), 0.0f, 0.0f));
			BackWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			BackWall->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));	
		}
		BackWall->WallMesh = this->WallMesh;
		BackWall->WallMaterial = this->WallMaterial;
		BackWall->WallMaterial2 = this->WallMaterial2;
		BackWall->MaxYOffset = this->MaxXOffset;
		BackWall->SetWall(RoomWidth, RoomHeight, 1);
	}
	else
	{
		if(BackWall != nullptr)
		{
			BackWall->Destroy();
		}
	}
	if(bCeiling)
	{
		if(Ceiling== nullptr)
		{
			Ceiling = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
			Ceiling->SetActorRelativeLocation(FVector(0.0f, 0.0f, RoomHeight*Ceiling->Spacing));
			Ceiling->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);	
		}
		Ceiling->WallMesh = this->WallMesh;
		Ceiling->WallMaterial = this->WallMaterial;
		Ceiling->WallMaterial2 = this->WallMaterial2;
		Ceiling->MaxZOffset = this->MaxZOffset;
		Ceiling->SetWall(RoomLength+1, 1, RoomWidth+1);
	}
	else
	{
		if(Ceiling != nullptr)
		{
			Ceiling->Destroy();
		}
	}
	if(bFloor)
	{
		if(Floor== nullptr)
		{
			Floor = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
			Floor->SetActorRelativeLocation(FVector(0.0f, 0.0f, Floor->Spacing*-1));
			Floor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);	
		}
		Floor->WallMesh = this->WallMesh;
		Floor->WallMaterial = this->WallMaterial;
		Floor->SetWall(RoomLength, 1, RoomWidth);
	}
	else
	{
		if(Floor != nullptr)
		{
			Floor->Destroy();
		}
	}
	
	
}

