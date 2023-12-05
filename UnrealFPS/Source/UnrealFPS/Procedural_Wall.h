// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Procedural_Wall.generated.h"

UCLASS()
class UNREALFPS_API AProcedural_Wall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcedural_Wall();
	void OnConstruction(const FTransform &Transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UInstancedStaticMeshComponent* WallMeshComponent;
	UInstancedStaticMeshComponent* WallMeshComponent2;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
    int32 WallLength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	int32 WallHeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	int32 WallWidth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	float MaxXOffset;
	UPROPERTY(editAnywhere, BlueprintReadOnly, Category = "Wall")
	float MaxYOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	float MaxZOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
	float Spacing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallLook")
	UStaticMesh* WallMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallLook")
	UMaterial* WallMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallLook")
	UMaterial* WallMaterial2;
	
	UFUNCTION(BlueprintCallable, Category = "Wall")
	void CreateWall();

	void SetWall(int Length, int Height, int Width);
	void ResetWall();
};
