// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapons/Weapon.h"

// Sets default values
AMainPlayerController::AMainPlayerController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling() && bIsSliding)
	{
		GetWorldTimerManager().PauseTimer(SlideHandle);
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(SlideHandle);
	}

	if (Weapon)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		Weapon->UpdateLoc(CameraLocation, CameraRotation);
	}
}

// Called to bind functionality to input
void AMainPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Jumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this,
		                                   &AMainPlayerController::Crouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMainPlayerController::Slide);
		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this,
		                                   &AMainPlayerController::Sprinting);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMainPlayerController::Shot);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,
		                                   &AMainPlayerController::StopShot);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AMainPlayerController::Reload);
		EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Triggered, this,
		                                   &AMainPlayerController::ChangeGun);
	}
}


/**
 * Move the player in the direction of the input
 * if w is pressed move forward if s is pressed move backwards etc.
 * @param Value Vector2D for X and Y
 */
void AMainPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// UE_LOG(LogTemp, Warning, TEXT("WASD pressed X: %f Y: %f"), MovementVector.X, MovementVector.Y);

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

/**
 * Look around
 * @param Value Vector2D for X and Y
 */
void AMainPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/// <summary>
/// when the jump button is pressed check if the player is sliding
/// if the player is not sliding jump normally
/// if the player is sliding add a force to the z axis to make the player jump but make sure the player is on the ground
/// </summary>
void AMainPlayerController::Jumping()
{
	//check if the player is sliding
	if (!bIsSliding)
	{
		//if the player is not sliding jump
		Jump();
	}
	else
	{
		//check if the player is on the ground
		if (!GetCharacterMovement()->IsFalling())
		{
			//get the direction the player is moving
			FVector Direction = GetCharacterMovement()->Velocity.GetSafeNormal();
			Direction.Z = 1.0f;
			//add a force to the direction the player is moving
			GetCharacterMovement()->AddImpulse(Direction * 600.0f, true);
		}
	}
}

/// <summary>
/// if the crouch button is pressed crouch
/// if the crouch button is released stand up
/// </summary>
void AMainPlayerController::Crouching()
{
	if (bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
		UnCrouch();
		bIsCrouched = false;
	}
	else
	{
		Crouch();
		bIsCrouched = true;
	}
}

/// <summary>
/// if the sprint button is pressed sprint
/// if the sprint button is released stop sprinting
/// </summary>
void AMainPlayerController::Sprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
}

/// <summary>
/// check if the player is moving fast enough to slide
/// if the player is moving fast enough slide by increasing the max walk speed
/// if the player is not moving fast enough stop sliding by decreasing the max walk speed
/// </summary>
void AMainPlayerController::Slide()
{
	if (GetCharacterMovement()->Velocity.Size() > 700.0f)
	{
		bIsSliding = true;
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 1200.0f;
		//after 2 seconds stop sliding
		GetWorldTimerManager().SetTimer(SlideHandle, this, &AMainPlayerController::StopSliding, 1.2f, false);
	}
	else
	{
		//cancel timer
		GetWorldTimerManager().ClearTimer(SlideHandle);
		bIsSliding = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	}
}

/**
 * Change the max walk speed back to normal
 * stop sliding
 */
void AMainPlayerController::StopSliding()
{
	bIsSliding = false;
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	}
	UnCrouch();
}

/**
 * start firing the weapon
 */
void AMainPlayerController::Shot()
{
	//create a timed loop to fire the weapon
	//GetWorldTimerManager().SetTimer(ShotHandle, this, &AMainPlayerController::fire, 0.1f, true);
	
	if (Weapon)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		Weapon->Shot(true, CameraLocation, CameraRotation, this);
	}
}

/**
 * stop firing the weapon
 */
void AMainPlayerController::StopShot()
{
	//debug
	// UE_LOG(LogTemp, Warning, TEXT("StopShot"));
	//GetWorldTimerManager().ClearTimer(ShotHandle);
	if (Weapon)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		Weapon->Shot(false, CameraLocation, CameraRotation, this);
	}
}

/**
 * Call the reload function in the weapon class
 */
void AMainPlayerController::Reload()
{
	if (Weapon)
	{
		Weapon->Reload();
	}
}

/**
 * change the weapon depending on the mouse wheel, reset the selected gun if it is out of bounds
 * stop firing the old weapon
 * loop through all the weapons and disable them
 * enable the selected weapon
 * @param Value if mouse wheel is scrolled up or down	
 */
void AMainPlayerController::ChangeGun(const FInputActionValue& Value)
{
	if (Value.Get<float>() > 0.0f)
	{
		++selectedGun;
	}
	else if (Value.Get<float>() < 0.0f)
	{
		--selectedGun;
	}
	if (selectedGun >= WeaponsArr.Num())
	{
		selectedGun = 0;
	}
	else if (selectedGun < 0)
	{
		selectedGun = WeaponsArr.Num() - 1;
	}
	StopShot();
	for (int i = 0; i < WeaponsArr.Num(); ++i)
	{
		if (i == selectedGun)
		{
			USceneComponent* WeaponComponent = Cast<USceneComponent>(WeaponsArr[i]);
			WeaponComponent->SetVisibility(true);
			//cast the weapon to AWeapon
			Weapon = Cast<AWeapon>(WeaponsArr[i]->GetChildActor());
			
		}
		else
		{
			USceneComponent* WeaponComponent = Cast<USceneComponent>(WeaponsArr[i]);
			WeaponComponent->SetVisibility(false);
		}
	}
	
}

/** DEPRECATED use GetAmmo and GetMaxAmmo instead
 * Only left here just in case it is needed
 * @return the ammo of the weapon as a string
 */
FString AMainPlayerController::GetAmmoString()
{
	if (Weapon)
	{
		return Weapon->GetAmmoString();
	}
	return FString("00/00");
}

/**
 * check if the weapon is not null if it is not null check the ammo of the weapon if it is less than 10 add a 0 to the front of the string
 * @return the ammo of the weapon as a string
 */
FString AMainPlayerController::GetAmmo()
{
	if (Weapon)
	{
		//if Ammo is less than 10 add a 0 to the front of the string
		if (Weapon->GetAmmo() < 10)
		{
			return FString("0" + FString::FromInt(Weapon->GetAmmo()));
		}
		return FString::FromInt(Weapon->GetAmmo());
	}
	return FString("00");
}

/**
 * if the weapon is not null check the max ammo of the weapon if it is less than 10 add a 0 to the front of the string
 * @return the max ammo of the weapon as a string
 */
FString AMainPlayerController::GetMaxAmmo()
{
	if (Weapon)
	{
		//if MaxAmmo is less than 10 add a 0 to the front of the string
		if (Weapon->GetMaxAmmo() < 10)
		{
			return FString("0" + FString::FromInt(Weapon->GetMaxAmmo()));
		}
		return FString::FromInt(Weapon->GetMaxAmmo());
	}
	return FString("00");
}

/**
 * @return the percentage of ammo left
 */
float AMainPlayerController::GetAmmoPer()
{
	if (Weapon)
	{
		float Ammo = Weapon->GetAmmo();
		float MaxAmmo = Weapon->GetMaxAmmo();
		return Ammo / MaxAmmo;
		// return Weapon->GetAmmo() / Weapon->GetMaxAmmo();
	}
	return 0.0f;
}


/**
 * Do not touch this function, a slight change can break the guns
 * loop through all the weapons and disable them
 * enable the first weapon
 */
void AMainPlayerController::SetUpGun()
{
	for (auto Element : WeaponsArr)
	{
		// Disable all weapons
		USceneComponent* WeaponComponent = Cast<USceneComponent>(Element);
		if (WeaponComponent)
		{
			WeaponComponent->SetVisibility(false);
		}
	}
	if(WeaponsArr.Num() > 0)
	{
		USceneComponent* WeaponComponent = Cast<USceneComponent>(WeaponsArr[0]);
		WeaponComponent->SetVisibility(true);
		Weapon = Cast<AWeapon>(WeaponsArr[0]->GetChildActor());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Weapons Found"));
	}
}