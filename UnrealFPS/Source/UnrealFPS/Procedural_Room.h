// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Procedural_wall.h"
#include "Procedural_Room.generated.h"

UCLASS()
class UNREALFPS_API AProcedural_Room : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcedural_Room();
	void OnConstruction(const FTransform &Transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	AProcedural_Wall* LeftWall;
    AProcedural_Wall* RightWall;
    AProcedural_Wall* FrontWall;
    AProcedural_Wall* BackWall;
    AProcedural_Wall* Ceiling;
    AProcedural_Wall* Floor;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int32 RoomLength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int32 RoomHeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int32 RoomWidth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	bool bCeiling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	bool bLeftWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	bool bRightWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	bool bFrontWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	bool bBackWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	bool bFloor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallLook")
	UStaticMesh* WallMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallLook")
	UMaterial* WallMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallLook")
	UMaterial* WallMaterial2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallLook")
	UMaterial* FloorMaterial;
	void CreateRoom();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	float MaxXOffset;
	UPROPERTY(editAnywhere, BlueprintReadOnly, Category = "Wall")
	float MaxYOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	float MaxZOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	float Spacing;
	
};
