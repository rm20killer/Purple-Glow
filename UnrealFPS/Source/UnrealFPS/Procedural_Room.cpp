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
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto AttachedActor : AttachedActors)
	{
		AttachedActor->Destroy();
	}
	if(LeftWall)
	{
		LeftWall->Destroy();
	}
	if(RightWall)
	{
		RightWall->Destroy();
	}
	if(FrontWall)
	{
		FrontWall->Destroy();
	}
	if(BackWall)
	{
		BackWall->Destroy();
	}
	if(Ceiling)
	{
		Ceiling->Destroy();
	}
	if(Floor)
	{
		Floor->Destroy();
	}
	//get child actors and nuke them
	TArray<AActor*> ChildActors;
	GetAttachedActors(ChildActors);
	for (auto ChildActor : ChildActors)
	{
		ChildActor->Destroy();
	}
	
	CreateRoom();
}

// Called every frame
void AProcedural_Room::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProcedural_Room::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	//get attached actors and nuke them
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto AttachedActor : AttachedActors)
	{
		AttachedActor->Destroy();
	}
	if(LeftWall)
	{
		LeftWall->Destroy();
	}
	if(RightWall)
	{
		RightWall->Destroy();
	}
	if(FrontWall)
	{
		FrontWall->Destroy();
	}
	if(BackWall)
	{
		BackWall->Destroy();
	}
	if(Ceiling)
	{
		Ceiling->Destroy();
	}
	if(Floor)
	{
		Floor->Destroy();
	}
	//get child actors and nuke them
	TArray<AActor*> ChildActors;
	GetAttachedActors(ChildActors);
	for (auto ChildActor : ChildActors)
	{
		ChildActor->Destroy();
	}
	
	CreateRoom();
}
void AProcedural_Room::CreateRoom()
{
	if(bLeftWall)
	{
		//spawn a wall and set its properties
		LeftWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
		//LeftWall->Rename(TEXT("LeftWall"));
		//attach the wall to the room
		LeftWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		LeftWall->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		LeftWall->WallMesh = this->WallMesh;
		LeftWall->WallMaterial = this->WallMaterial;
		LeftWall->WallMaterial2 = this->WallMaterial2;
		LeftWall->MaxYOffset = this->MaxYOffset;
		LeftWall->SetWall(RoomLength, RoomHeight, 1);
		//rename the wall
	}
	if(bRightWall)
	{
		//spawn a wall and set its properties
		RightWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
		//RightWall->Rename(TEXT("RightWall"));
		//attach the wall to the room
		RightWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		//move to wall on y y axis by RoomWidth
		RightWall->SetActorRelativeLocation(FVector(0.0f, RoomWidth*(RightWall->Spacing-1), 0.0f));
		RightWall->WallMesh = this->WallMesh;
		RightWall->WallMaterial = this->WallMaterial;
		RightWall->WallMaterial2 = this->WallMaterial2;
		RightWall->MaxYOffset = this->MaxYOffset;
		RightWall->SetWall(RoomLength, RoomHeight, 1);
	}
	if(bFrontWall)
	{
		//spawn a wall and set its properties
		FrontWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
		//FrontWall->Rename(TEXT("FrontWall"));
		//move to wall on y y axis by RoomWidth
		FrontWall->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		FrontWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		//rotate the wall 90
		FrontWall->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		FrontWall->WallMesh = this->WallMesh;
		FrontWall->WallMaterial = this->WallMaterial;
		FrontWall->WallMaterial2 = this->WallMaterial2;
		FrontWall->MaxYOffset = this->MaxXOffset;
		FrontWall->SetWall(RoomWidth, RoomHeight, 1);
	}
	if(bBackWall)
	{
		//spawn a wall and set its properties
		BackWall = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
		//BackWall->Rename(TEXT("BackWall"));
		//make the wall a child of the room
		BackWall->SetActorRelativeLocation(FVector(RoomLength*(BackWall->Spacing-1), 0.0f, 0.0f));
		BackWall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		//rotate the wall 90
		BackWall->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		BackWall->WallMesh = this->WallMesh;
		BackWall->WallMaterial = this->WallMaterial;
		BackWall->WallMaterial2 = this->WallMaterial2;
		BackWall->MaxYOffset = this->MaxXOffset;
		BackWall->SetWall(RoomWidth, RoomHeight, 1);
	}
	if(bCeiling)
	{
		//spawn a wall and set its properties
		Ceiling = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
		//Ceiling->Rename(TEXT("Ceiling"));
		//attach the wall to the room
		Ceiling->SetActorRelativeLocation(FVector(0.0f, 0.0f, RoomHeight*Ceiling->Spacing));
		Ceiling->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Ceiling->WallMesh = this->WallMesh;
		Ceiling->WallMaterial = this->WallMaterial;
		Ceiling->WallMaterial2 = this->WallMaterial2;
		Ceiling->MaxZOffset = this->MaxZOffset;
		Ceiling->SetWall(RoomLength+1, 1, RoomWidth+1);
	}
	if(bFloor)
	{
		//spawn a wall and set its properties
		Floor = GetWorld()->SpawnActor<AProcedural_Wall>(AProcedural_Wall::StaticClass(), GetActorLocation(), GetActorRotation());
		//Floor->Rename(TEXT("Floor"));
		//attach the wall to the room
		Floor->SetActorRelativeLocation(FVector(0.0f, 0.0f, Floor->Spacing*-1));
		Floor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Floor->WallMesh = this->WallMesh;
		Floor->WallMaterial = this->WallMaterial;
		Floor->SetWall(RoomLength, 1, RoomWidth);
	}
	
	
}

