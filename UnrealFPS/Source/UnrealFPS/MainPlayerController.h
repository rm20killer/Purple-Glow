// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputLibrary.h"
#include "MainPlayerController.generated.h"

UCLASS()
class UNREALFPS_API AMainPlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsCrouched = false;
	bool bIsSprinting = false;
	bool bIsSliding = false;
	FTimerHandle SlideHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	class UInputMappingContext* DefaultMappingContext;

	//Jump button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	class UInputAction* JumpAction;

	//Move button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	class UInputAction* MoveAction;

	//Look button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	class UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	class UInputAction* FireAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Jumping();

	void Crouching();

	void Sprinting();

	void Slide();

	void StopSliding();

	void Shot();
	
};
