// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputLibrary.h"
#include "Containers/Array.h"
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
	FTimerHandle ShotHandle;
	AWeapon* Weapon;
	int selectedGun = 0;
	// UUserWidget* AmmoWidgetInstance;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	// UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	// TSubclassOf<UUserWidget> AmmoWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputMappingContext* DefaultMappingContext;


	//movement
	// - input action
	//Jump button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* JumpAction;
	//Move button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* MoveAction;
	//Look button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* LookAction;
	//crouch button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* CrouchAction;
	//sprint button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* SprintAction;

	// - Move functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jumping();
	void Crouching();
	void Sprinting();
	void Slide();
	void StopSliding();

	//Weapons
	// - weapons input
	//fire button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* FireAction;
	//reload button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* NextWeaponAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon")
	TArray<UChildActorComponent*> WeaponsArr;
	// - weapons functions
	void Shot();
	void StopShot();
	void Reload();
	void ChangeGun(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FString GetAmmoString();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FString GetAmmo();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FString GetMaxAmmo();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetAmmoPer();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetUpGun();

	
};